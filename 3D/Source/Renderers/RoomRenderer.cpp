#include "Renderers/RoomRenderer.h"

#include "SeatGrid.h"

void RoomRenderer::draw(const glm::mat4& projection,
                        const glm::mat4& view,
                        CubeRenderer& renderer,
                        const glm::vec3& cameraPos,
                        const glm::vec3& cameraFront,
                        float doorOpen,
                        bool showStaticScreen)
{
    renderer.setMatrices(projection, view);

    // FLOOR
    renderer.drawCube(
        {0.0f, -0.1f, 6.0f},
        {8.0f, 0.2f, 20.0f},
        {0.20f, 0.20f, 0.20f}
    );


    //BACK WALL
    renderer.drawCube(
        {0.0f, 3.0f, -2.0f},
        {8.0f, 6.2f, 0.2f},
        {0.10f, 0.10f, 0.10f}
    );

    // LEFT WALL
    renderer.drawCube(
        {-4.0f, 3.0f, 6.0f},
        {0.2f, 6.2f, 20.0f},
        {0.15f, 0.15f, 0.15f}
    );

    // RIGHT WALL
    renderer.drawCube(
        {4.0f, 3.0f, 6.0f},
        {0.2f, 6.2f, 20.0f},
        {0.15f, 0.15f, 0.15f}
    );

    // CEILING
    renderer.drawCube(
        {0.0f, 6.2f, 6.0f},
        {8.0f, 0.2f, 20.0f},
        {0.18f, 0.18f, 0.18f}
    );

    if (showStaticScreen)
        drawScreen(renderer);
    drawDoors(renderer, doorOpen);
    // drawSeats(renderer, cameraPos, cameraFront);
}

void RoomRenderer::drawScreen(CubeRenderer& renderer)
{
    //Cinema screen
    renderer.drawCube(
            {0.0f, 2.6f, -1.80f},
            {6.6f, 3.6f, 0.02f},
            {1.0f, 1.0f, 1.0f}
        );
}

void RoomRenderer::drawDoors(CubeRenderer& renderer, float doorOpen)
{
    const glm::vec3 doorColor = {0.25f, 0.15f, 0.05f};
    float openAngle = doorOpen * glm::radians(100.0f);

    // Left wall doors
    renderer.drawCubeRotatedY(
        {-3.88f, 0.9f, -1.45f},
        {0.1f, 1.8f, 0.45f},
        doorColor,
        openAngle
    );
    renderer.drawCubeRotatedY(
        {-3.88f, 0.9f, -0.95f},
        {0.1f, 1.8f, 0.45f},
        doorColor,
        -openAngle
    );

    // Right wall doors
    renderer.drawCubeRotatedY(
        {3.88f, 0.9f, -1.45f},
        {0.1f, 1.8f, 0.45f},
        doorColor,
        -openAngle
    );
    renderer.drawCubeRotatedY(
        {3.88f, 0.9f, -0.95f},
        {0.1f, 1.8f, 0.45f},
        doorColor,
        openAngle
    );
}

