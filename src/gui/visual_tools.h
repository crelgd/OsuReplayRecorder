/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

namespace visualTools
{
    class vbo
    {
    public:
        int init(float* data, size_t dataSize, int vertexCount, GLuint drawType);
        ~vbo();

        GLuint obj;
        bool status;
    };

    class shader
    {
    public:
        int init(GLuint shaderType, const char* code, int codeLines);
        ~shader();

        GLuint obj;
        bool status;
    };

    class program
    {
    public:
        int init(GLuint vertShader, GLuint fragShader);
        ~program();

        GLuint obj;
        bool status;
    };

    class vao
    {
    public:
        int init(GLuint vbo);
        ~vao();

        GLuint obj;
        bool status;
    };
}