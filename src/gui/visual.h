/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#pragma once

#include <ctime>

#include "visual_tools.h"
#include "visual_obj.h"
#include "osr.h"

namespace visual 
{
    class window
    {
    public:
        window();
        ~window();
        void run();
        void osrSet(const char* file);

    private:
        void ResizeEvent();
        void objsInit();
    private:
        SDL_Window* hwnd;
        SDL_GLContext glContext;
        int wW = 500;
        int wH = 500;
        SDL_Event e;
        bool proc = true;

        uint32_t tCur = SDL_GetTicks();
        uint32_t time = 0;

        int curPlay = 0;

        osr::OsrFile osrf;

        Circle cursor;
        std::vector<osr::Decompile> gDec;
    };
}
