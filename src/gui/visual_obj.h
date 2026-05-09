#include "visual.h"

namespace visual
{
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

    class Circle : public Object
    {
    public:
        int init(float x, float y, float r);
        void draw();

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
}
