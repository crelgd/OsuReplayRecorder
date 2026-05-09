/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include "gui/visual.h"
#include "base.h"

#include <cmath>

// не сматрите на код
// 
// туду сделать изменение расположения курсора

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

        try{ objsInit(); }
        catch (std::runtime_error& err) {
            IFEL(1, err.what());
        }
    }

    void window::objsInit()
    {
        osrf.Read();
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
        
        std::vector<osr::Decompile> decompileBfr = osr::ReadDecompile(fullData);
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

        note.resize(decompileBfr.size());
        for (int i = 0; i < note.size(); i++)
        {
            note[i].init(decompileBfr[i].x / 512.0f, decompileBfr[i].y / 384.0f, 0.1);
        }
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
                            ResizeEvent();
                        }
                    }   
                    break;
                }
            }
            
            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            std::time_t tCur = std::time(0);

            for (int i = 0; i < note.size(); i++)
            {
                if ((tCur - time) >= gDec[i].w)
                {
                    note[i].draw();
                }
            }

            SDL_GL_SwapWindow(hwnd);
            SDL_Delay(16);
        }
    }

    GLuint Object::GetProgram()
    {
        return hShader.obj;
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
