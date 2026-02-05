#include "Renderers/PersonRenderer.h"
#include "Renderers/CubeRenderer.h"

void PersonRenderer::draw(CubeRenderer& renderer, const glm::vec3& basePos, unsigned int textureId)
{
    // Torso (textured)
    renderer.drawCubeTextured(basePos + glm::vec3(0.0f, 0.50f, 0.0f), glm::vec3(0.28f, 0.40f, 0.18f), textureId);

    // Head (textured)
    renderer.drawCubeTextured(basePos + glm::vec3(0.0f, 0.88f, 0.0f), glm::vec3(0.20f, 0.20f, 0.20f), textureId, glm::vec3(1.08f));

    // Legs
    renderer.drawCube(basePos + glm::vec3(-0.07f, 0.20f, 0.0f), glm::vec3(0.10f, 0.36f, 0.10f), glm::vec3(0.15f, 0.15f, 0.16f));
    renderer.drawCube(basePos + glm::vec3( 0.07f, 0.20f, 0.0f), glm::vec3(0.10f, 0.36f, 0.10f), glm::vec3(0.15f, 0.15f, 0.16f));

    // Arms
    renderer.drawCube(basePos + glm::vec3(-0.20f, 0.53f, 0.0f), glm::vec3(0.08f, 0.30f, 0.08f), glm::vec3(0.55f, 0.42f, 0.35f));
    renderer.drawCube(basePos + glm::vec3( 0.20f, 0.53f, 0.0f), glm::vec3(0.08f, 0.30f, 0.08f), glm::vec3(0.55f, 0.42f, 0.35f));
}
