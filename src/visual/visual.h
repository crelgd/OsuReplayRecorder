/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#pragma once

#include <ctime>

#include "visual/visual_tools.h"
#include "osr.h"

namespace visual 
{
    class Circle : public base::Object
    {
    public:
        int init(float x, float y, float r);
        void draw();
        void ChangePos(float x, float y, float r);

    private:
        int vertexes = 0;

        const char* vS = "\
        #version 330 core\n\
        layout (location = 0) in vec3 aPos;\n\
        void main()\n\
        {\n\
        gl_Position =vec4(aPos.xy, 0.0, 1.0);\n\
        }";

        const char* fS = "\
        #version 330 core\n\
        out vec4 FragColor;\n\
        void main()\n\
        {\n\
        FragColor = vec4(1.0f, 0.0f, 0.0f, 0.0f);\n\
        }"; 
    };

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
        std::vector<OsrDecompile> ReadDecompile(std::vector<uint8_t>& bfr);
    private:
        SDL_Window* hwnd;
        SDL_GLContext glContext;
        SDL_Event e;

        int wW = 500;
        int wH = 500;
        bool proc = true;

        uint32_t tCur = SDL_GetTicks();
        uint32_t time = 0;
        int curPlay = 0;

        osr::OsrFile osrf;
        Circle cursor;

        std::vector<OsrDecompile> gDec;
    };
}
