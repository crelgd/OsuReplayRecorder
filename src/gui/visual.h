/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#pragma once

#define WINDOW_W    500
#define WINDOW_H    500

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
        SDL_GLContext glContext;
        SDL_Event e;
        bool proc = true;
    };
}
