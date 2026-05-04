/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include "visual.h"

namespace visual
{
    uint32_t CreateVBO(float* data, size_t dataSize, int vertexCount, uint16_t drawType)
    {
        uint32_t vbo;

        if (vertexCount < 1)
            return 0;

        // если не заданы 3 значения для каждой вершины
        if ((dataSize % (3 * sizeof(float))) != 0)
            return 0;

        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, dataSize, data, drawType);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return vbo;
    }

    uint32_t ShaderCompile(uint16_t shaderType, const char* code, int codeLines)
    {
        uint32_t shader = glCreateShader(shaderType);

        if (!shader)
            return 0;

        glShaderSource(shader, codeLines, &code, NULL);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
            return 0;

        return shader;
    }

    uint32_t CreateProgram(uint32_t vertShader, uint32_t fragShader)
    {
        uint32_t program = glCreateProgram();

        glAttachShader(program, vertShader);
        glAttachShader(program, fragShader);
        glLinkProgram(program);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (!success)
            return 0;

        return program;
    }

    uint32_t CreateVAO(uint32_t vbo)
    {
        uint32_t vao;
        glGenVertexArrays(1, &vao);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        return vao;
    }
}
