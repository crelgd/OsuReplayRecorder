/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include "gui/visual.h"

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
                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        
        if (!hwnd)
            throw std::runtime_error("Окно SDL не было создано");
    }

    window::~window()
    {
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

            SDL_Delay(1);
        }
    }
}