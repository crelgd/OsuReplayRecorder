/*
 * MIT License
 * Copyright (c) 2026 crelgd
 */

#include "gui/visual.h"
#include "base.h"
#include "SDL/SDL_opengl.h"  

namespace visual
{
    window::window()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
            throw std::runtime_error("SDL не был инициализирован");

        hwnd = SDL_CreateWindow(
                "Osu Replay Reader", 
                SDL_WINDOWPOS_UNDEFINED, 
                SDL_WINDOWPOS_UNDEFINED, 
                wW, wH, 
                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);        
        IFEL(hwnd, "Окно SDL не было создано");

        glContext = SDL_GL_CreateContext(hwnd);
        IFEL(glContext, "Контекст OpenGL не был создан");

        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (GLEW_OK != err)
            throw std::runtime_error("GLEW не был инициализирован");

        SDL_AddEventWatch(ResizeEvent, hwnd);

        float triangleVertex[] = {
            -0.5, -0.5, 1,
            0.5, 0.5, 1,
            0.5, -0.5, 1
        };

        uint32_t trglVBO = CreateVBO(triangleVertex, sizeof(triangleVertex), 3, GL_STATIC_DRAW);
        IFEL(trglVBO, "VBO треугольника не было создано");

        const char* vert = "\
        #version 330 core\n\
        layout (location = 0) in vec3 aPos;\n\
        out vec4 vertexColor;\n\
        void main()\n\
        {\n\
            gl_Position = vec4(aPos, 1.0);\n\
            vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n\
        }";

        const char* frag = "\
        #version 330 core\n\
        out vec4 FragColor;\n\
        in vec4 vertexColor;\n\
        void main()\n\
        {\n\
            FragColor = vertexColor;\n\
        }";

        uint32_t fragS = ShaderCompile(GL_FRAGMENT_SHADER, frag, 1);
        uint32_t vertS = ShaderCompile(GL_VERTEX_SHADER, vert, 1);

        triangleProg = CreateProgram(vertS, fragS);
        IFEL(triangleProg, "Шейдеры не были соеденины");

        triangle = CreateVAO(trglVBO);
    }

    window::~window()
    {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(hwnd);
        SDL_Quit();
    }

    void window::run()
    {
        while (proc)
        {
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    proc = false;
                }
            }
            
            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(triangleProg);
            glBindVertexArray(triangle);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            SDL_GL_SwapWindow(hwnd);
            SDL_Delay(16);
        }
    }
}
