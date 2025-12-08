#include "../Header/Renderers/SeatRenderer.h"
#include "../Header/glad/glad.h"
#include "../Header/Util.h"

// VBO da ne bi izgubio handler
static unsigned int g_seatVBO = 0;

void initSeatRendering(unsigned int& seatShader, unsigned int& seatVAO)
{
    seatShader = createShader("../Shaders/seat.vert", "../Shaders/seat.frag");

    float seatVerts[] = {
        -1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f
    };

    glGenVertexArrays(1, &seatVAO);
    glGenBuffers(1, &g_seatVBO);

    glBindVertexArray(seatVAO);
    glBindBuffer(GL_ARRAY_BUFFER, g_seatVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(seatVerts), seatVerts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
