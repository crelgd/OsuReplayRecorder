/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include "gui/visual.h"
#include "base.h"

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

        SDL_AddEventWatch(ResizeEvent, hwnd);

        try{ objsInit(); }
        catch (std::runtime_error& err) {
            IFEL(1, err.what());
        }
    }

    void window::objsInit()
    {
        IFEL(crcl.init(0, 0, 0.5) != 0, "Круг не был создан");
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
                if (e.type == SDL_QUIT)
                {
                    proc = false;
                }
            }
            
            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            crcl.draw();

            SDL_GL_SwapWindow(hwnd);
            SDL_Delay(16);
        }
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
