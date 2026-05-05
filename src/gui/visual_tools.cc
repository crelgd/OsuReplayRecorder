/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include "visual_tools.h"
#include "base.h"

namespace visualTools
{
    int vbo::init(float* data, size_t dataSize, int vertexCount, GLuint drawType)
    {
        if (vertexCount < 1)
            return 1;
        // если не заданы 3 значения для каждой вершины
        if ((dataSize % (3 * sizeof(float))) != 0)
            return 1;

        glGenBuffers(1, &obj);

        glBindBuffer(GL_ARRAY_BUFFER, obj);
        glBufferData(GL_ARRAY_BUFFER, dataSize, data, drawType);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return 0;
    }

    vbo::~vbo()
    {
        glDeleteBuffers(1, &obj);
    }

    int shader::init(GLuint shaderType, const char* code, int codeLines)
    {
        obj = glCreateShader(shaderType);

        if (!obj)
            return 1;

        glShaderSource(obj, codeLines, &code, NULL);
        glCompileShader(obj);

        int success;
        glGetShaderiv(obj, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            char log[512];
            glGetShaderInfoLog(obj, 512, NULL, log);
            spdlog::error("SHADER ERROR: {}", log);
            return 1;
        }

        return 0;
    }

    shader::~shader()
    {
        glDeleteShader(obj);
    }

    int program::init(GLuint vertShader, GLuint fragShader)
    {
        obj = glCreateProgram();

        glAttachShader(obj, vertShader);
        glAttachShader(obj, fragShader);
        glLinkProgram(obj);

        int success;
        glGetProgramiv(obj, GL_LINK_STATUS, &success);

        if (!success)
            return 1;

        return 0;
    }

    program::~program()
    {
        glDeleteProgram(obj);
    }

    int vao::init(GLuint vbo)
    {
        glGenVertexArrays(1, &obj);

        glBindVertexArray(obj);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        return 0;
    }

    vao::~vao()
    {
        glDeleteVertexArrays(1, &obj);
    }
}
