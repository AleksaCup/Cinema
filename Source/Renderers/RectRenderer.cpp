#include "../../Header/Renderers/RectRenderer.h"
#include "../../Header/Util.h"

void RectRenderer::init()
{
    shader = createShader("../Shaders/rect.vert", "../Shaders/rect.frag");

    float verts[] = {
        -1,  1,
        -1, -1,
         1, -1,
         1,  1
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void RectRenderer::draw(float x, float y, float w, float h, float r, float g, float b, float a)
{
    glUseProgram(shader);
    glUniform4f(glGetUniformLocation(shader, "uColor"), r, g, b, a);
    glUniform4f(glGetUniformLocation(shader, "uRect"), x, y, w, h);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
