/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include "gui/visual.h"
#include "base.h"
#include "SDL/SDL_opengl.h"  

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
                WINDOW_W, WINDOW_H, 
                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);        
        IFEL(hwnd, "Окно SDL не было создано");

        glContext = SDL_GL_CreateContext(hwnd);
        IFEL(glContext, "Контекст OpenGL не был создан");

        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (GLEW_OK != err)
            throw std::runtime_error("GLEW не был инициализирован");
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



            SDL_GL_SwapWindow(hwnd);
            SDL_Delay(16);
        }
    }
}
