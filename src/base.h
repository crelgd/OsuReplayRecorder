/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#pragma once

#define IFEL(val, msg) { if (val) throw std::runtime_error(std::string(msg)); }

#include "visual/visual_tools.h"

namespace base
{
    class File
    {
    public:
        void load(const char* filename);

    protected:
        size_t fileSize;
        uint64_t fileOffset = 0;
        std::vector<uint8_t> fileData;
    };

    class Object
    {
    public:
        float oX, oY;
        float oW, oH;
    public:
        GLuint GetProgram();

    protected:
        visualTools::vbo vertex;
        visualTools::program hShader;
        visualTools::vao obj;
    };
}

