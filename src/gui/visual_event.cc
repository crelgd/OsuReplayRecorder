/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include "visual.h"
#include "SDL/SDL.h"

namespace visual
{
    int window::ResizeEvent(void* data, SDL_Event* e)
    {
        SDL_Window* win = static_cast<SDL_Window*>(data);

        if (e->type == SDL_WINDOWEVENT && 
                e->window.event == SDL_WINDOWEVENT_RESIZED)
        {
            // дапустим окно даступна постоянно

            int wW, wH;
            SDL_GetWindowSize(win, &wW, &wH);
            glViewport(0, 0, wW, wH);
        }

        return 0;   
    }
}
