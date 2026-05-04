/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#pragma once

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
    private:
        SDL_Window* hwnd;
        SDL_GLContext glContext;
        int wW = 500;
        int wH = 500;
        SDL_Event e;
        bool proc = true;

        uint32_t triangle;
        uint32_t triangleProg;
    };

    uint32_t CreateVBO(float* data, size_t dataSize, int vertexCount, uint16_t drawType);
    uint32_t ShaderCompile(uint16_t shaderType, const char* code, int codeLines);
    // не забыть что шейдеры удаляются
    // что использовать шейдер нужно glUseProgram()
    uint32_t CreateProgram(uint32_t vertShader, uint32_t fragShader);
    uint32_t CreateVAO(uint32_t vbo);
}
