#include "../../Header/glad/glad.h"
#include "../../Header/Util.h"
#include "../../Header/Renderers/PersonRenderer.h"

void PersonRenderer::init()
{
    shader = createShader("../Shaders/person.vert", "../Shaders/person.frag");
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "uTex"), 0);

    float verts[] = {
        // pos     // uv
        -1.0f,  1.0f,  0.0f, 0.0f,
        -1.0f, -1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 0.0f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void PersonRenderer::draw(float x, float y, float size, unsigned int texID, bool flipY)
{
    glUseProgram(shader);

    glUniform2f(glGetUniformLocation(shader, "uPos"),  x, y);
    glUniform1f(glGetUniformLocation(shader, "uSize"), size);
    glUniform1i(glGetUniformLocation(shader, "uFlipY"), flipY ? 1 : 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


