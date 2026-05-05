/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#pragma once

#include "visual_tools.h"
#include "visual_obj.h"

namespace visual 
{
    class window
    {
    public:
        window();
        ~window();
        void run();

    private:
        static int ResizeEvent(void* data, SDL_Event* e);
        void objsInit();
    private:
        SDL_Window* hwnd;
        SDL_GLContext glContext;
        int wW = 500;
        int wH = 500;
        SDL_Event e;
        bool proc = true;

        Circle crcl;
    };
}
