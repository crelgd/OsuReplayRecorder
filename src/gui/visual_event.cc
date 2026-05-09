/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include "visual.h"
#include "SDL/SDL.h"

namespace visual
{
    void window::ResizeEvent()
    {
        SDL_GetWindowSize(hwnd, &wW, &wH);
        glViewport(0, 0, wW, wH);
    }
}
