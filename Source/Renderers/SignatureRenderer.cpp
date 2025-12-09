#include "../../Header/Renderers/SignatureRenderer.h"
#include "../../Header/Util.h"
#include <glad/glad.h>

static unsigned int g_signatureVBO = 0; // da ne iscuri iz scopa

void initSignatureRendering(unsigned int& shader, unsigned int& vao, unsigned int& tex, int screenW, int screenH)
{
    tex = loadImageToTexture("../Resources/signature.png");
    shader = createShader("../Shaders/signature.vert", "../Shaders/signature.frag");

    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "uTex"), 0);

    int sigW = 500;
    int sigH = 128;
    int margin = 20;

    float x1 = screenW  - sigW - margin;
    float y1 = screenH - sigH - margin;
    float x2 = x1 + sigW;
    float y2 = y1 + sigH;

    float x1N = (x1 / screenW) * 2.0f - 1.0f;
    float x2N = (x2 / screenW) * 2.0f - 1.0f;
    float y1N = -((y1 / screenH) * 2.0f - 1.0f);
    float y2N = -((y2 / screenH) * 2.0f - 1.0f);

    float verts[] = {
        // pos       // uv
        x1N, y2N,    0.0f, 0.0f, // top-left
        x1N, y1N,    0.0f, 1.0f, // bottom-left
        x2N, y1N,    1.0f, 1.0f, // bottom-right
        x2N, y2N,    1.0f, 0.0f  // top-right
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &g_signatureVBO);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_signatureVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    // pozicija
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void drawSignatureRendering(unsigned int shader, unsigned int vao, unsigned int tex)
{
    glUseProgram(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
