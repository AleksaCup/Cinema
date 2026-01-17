#include "Renderers/RoomRenderer.h"

void RoomRenderer::draw(const glm::mat4& projection,
                        const glm::mat4& view,
                        CubeRenderer& renderer)
{
    renderer.setMatrices(projection, view);

    // Floor
    renderer.drawCube({0.0f, -0.1f, 3.0f}, {8.0f, 0.2f, 12.0f}, {0.2f,0.2f,0.2f});

    // Back wall - screen wall
    renderer.drawCube({0.0f, 2.5f, -2.0f}, {8.0f, 5.0f, 0.2f}, {0.1f,0.1f,0.1f});

    // Left wall
    renderer.drawCube({-4.0f, 2.5f, 3.0f}, {0.2f, 5.0f, 12.0f}, {0.15f,0.15f,0.15f});

    // Right wall
    renderer.drawCube({4.0f, 2.5f, 3.0f}, {0.2f, 5.0f, 12.0f}, {0.15f,0.15f,0.15f});

    // Ceiling
    renderer.drawCube({0.0f, 5.1f, 3.0f}, {8.0f, 0.2f, 12.0f}, {0.18f,0.18f,0.18f});
}
