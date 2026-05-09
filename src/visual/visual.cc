/*
* MIT License
* Copyright (c) 2026 crelgd
*/

#include "visual/visual.h"
#include "base.h"
#include "osr.h"

#include <cmath>

namespace visual
{
    window::window()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
            throw std::runtime_error("SDL не был инициализирован");

        hwnd = SDL_CreateWindow(
                "Osu Replay Reader", 
                SDL_WINDOWPOS_UNDEFINED, 
                SDL_WINDOWPOS_UNDEFINED, 
                wW, wH, 
                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);        
        IFEL(!hwnd, "Окно SDL не было создано");

        glContext = SDL_GL_CreateContext(hwnd);
        IFEL(!glContext, "Контекст OpenGL не был создан");

        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (GLEW_OK != err)
            throw std::runtime_error("GLEW не был инициализирован");
    }

    void window::osrSet(const char* file)
    {
        osrf.load(file);
        osrf.Read();

        try{ objsInit(); }
        catch (std::runtime_error& err) {
            IFEL(1, err.what());
        }
    }

    void window::objsInit()
    {
        OsrErr err = OSR_OK;

        std::vector<uint8_t> decBfr(8192);

        err = osrf.DecodeInit();
        if (err != OSR_OK)
            IFEL(true, "Decoder init err");

        std::vector<uint8_t> fullData;

        while (err != OSR_DECODE_END)
        {
            size_t written;
            err = osrf.Decode(decBfr, written);

            if (err == OSR_ERR)
                IFEL(true, "Decode err");
            if (err == OSR_DECODE_END)
                break;

            fullData.insert(fullData.end(), decBfr.begin(), decBfr.begin() + written);
        }
            
        std::vector<OsrDecompile> decompileBfr = ReadDecompile(fullData);

            // for (int i = 0; i < decompileBfr.size(); i++)
            // {
            //     std::cout << 
            //         "w:" << decompileBfr[i].w << "\n" <<
            //         "x:" << decompileBfr[i].x / 512.0f << "\n" <<
            //         "y:" << decompileBfr[i].y / 384.0f << "\n" <<
            //         "z:" << decompileBfr[i].z << "\n";
            // }
            // std::cout << std::endl;

        gDec = decompileBfr;

        cursor.init(0, 0, 0.1);
    }

    window::~window()
    {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(hwnd);
        SDL_Quit();
    }

    void window::run()
    {
        while (proc)
        {
            while (SDL_PollEvent(&e) != 0)
            {
                switch (e.type)
                {
                case SDL_QUIT:
                    {
                        proc = false;
                    }   
                    break;

                case SDL_WINDOWEVENT:
                    {
                        if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                        {
                            SDL_GetWindowSize(hwnd, &wW, &wH);
                            glViewport(0, 0, wW, wH);
                        }
                    }   
                    break;
                }
            }
                
            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            uint32_t mTime = SDL_GetTicks();
            time += (mTime - tCur);

            if (curPlay < gDec.size() && gDec[curPlay].w < 0) curPlay++;

            if (curPlay < gDec.size() && time >= gDec[curPlay].w)
            {
                cursor.ChangePos((gDec[curPlay].x / 512.0f) * 2.0f - 1.0f, (gDec[curPlay].y / 384.0f) * 2.0f - 1.0f, 0.1);
                curPlay++;
                time = 0;

                if (curPlay >= gDec.size())
                    proc = false;
            }

            // std::cout << curPlay << " | " << gDec.size() << " time: " << time << " w: " << gDec[curPlay].w << std::endl;

             cursor.draw();

            tCur = mTime;

            SDL_GL_SwapWindow(hwnd);
            SDL_Delay(16);
        }
    }

    std::vector<OsrDecompile> window::ReadDecompile(std::vector<uint8_t>& bfr)
    {
        std::vector<OsrDecompile> outData;
        std::vector<char> numBfr;

        std::vector<float> readed;

        for (int i = 0; i < bfr.size(); i++)
        {
            if (bfr[i] == '|' || bfr[i] == ',') {
                std::string tmp(numBfr.begin(), numBfr.end());
                readed.push_back(std::stof(tmp));
                numBfr.clear();
            }
            else numBfr.push_back(bfr[i]);

            if (readed.size() == 4)
            {
                OsrDecompile dec; 
                dec.w = int(readed[0]);
                dec.x = readed[1];
                dec.y = readed[2];
                dec.z = int(readed[3]);

                outData.push_back(dec);

                readed.clear();
            }
        }

        return outData;
    }

    int Circle::init(float x, float y, float r)
    {
        std::vector<float> circleData;
        int err;

        for (int i = 0; i < 50; i++)
        {
            float theta = 2.0f * 3.1415926f * float(i) / float(50);
            float xx = r * cosf(theta);
            float yy = r * sinf(theta);

            circleData.push_back(x+xx);
            circleData.push_back(y+yy);
            circleData.push_back(0.0f);
        }

        err = vertex.init(static_cast<float*>(circleData.data()), circleData.size() * sizeof(float), 
            3, GL_STATIC_DRAW);
            
        if (err != 0)
            return 1;

        vertexes = circleData.size() / 3;

        visualTools::shader vShader; err = vShader.init(GL_VERTEX_SHADER, vS, 1);
        visualTools::shader fShader; err = fShader.init(GL_FRAGMENT_SHADER, fS, 1);

        err = hShader.init(vShader.obj, fShader.obj);

        if (err != 0)
            return 1;

        obj.init(vertex.obj);

        return 0;
    }

    void Circle::ChangePos(float x, float y, float r)
    {
        glBindVertexArray(obj.obj);
        glBindBuffer(GL_ARRAY_BUFFER, vertex.obj);

        std::vector<float> circleData;

        for (int i = 0; i < 50; i++)
        {
            float theta = 2.0f * 3.1415926f * float(i) / float(50);
            float xx = r * cosf(theta);
            float yy = r * sinf(theta);

            circleData.push_back(x+xx);
            circleData.push_back(y+yy);
            circleData.push_back(0.0f);
        }

        glBufferSubData(GL_ARRAY_BUFFER, 0, circleData.size() * sizeof(float), circleData.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Circle::draw()
    {
        if (vertexes == 0)
            return;

        glUseProgram(hShader.obj);
        glBindVertexArray(obj.obj);
        glDrawArrays(GL_LINE_LOOP, 0, vertexes);
        glBindVertexArray(0); 
    }
}
