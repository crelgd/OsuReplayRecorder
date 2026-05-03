/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#pragma once

#define WINDOW_W    1280
#define WINDOW_H    720

namespace visual 
{
    class window
    {
    public:
        window();
        ~window();
        void run();

    private:
        SDL_Window* hwnd;
        SDL_Event e;
        bool proc = true;
    };
}
