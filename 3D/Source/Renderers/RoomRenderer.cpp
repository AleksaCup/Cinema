#include "Renderers/RoomRenderer.h"

#include "SeatGrid.h"

void RoomRenderer::draw(const glm::mat4& projection,
                        const glm::mat4& view,
                        CubeRenderer& renderer,
                        const glm::vec3& cameraPos,
                        const glm::vec3& cameraFront,
                        float doorOpen)
{
    renderer.setMatrices(projection, view);

    // FLOOR
    renderer.drawCube(
        {0.0f, -0.1f, 6.0f},
        {8.0f, 0.2f, 20.0f},
        {0.20f, 0.20f, 0.20f}
    );


    //BACK WALL (screen wall)
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

    drawScreen(renderer);
    drawDoors(renderer, doorOpen);
    // drawSeats(renderer, cameraPos, cameraFront);
}

// void RoomRenderer::drawSeats(
//     CubeRenderer& renderer,
//     const glm::vec3& cameraPos,
//     const glm::vec3& cameraFront
// )
// {
//     const int ROWS = 12;
//     const int COLS = 10;
//
//     const float SEAT_W = 0.55f;
//     const float SEAT_H = 0.6f;
//     const float SEAT_D = 0.55f;
//
//     const float STEP_H = 0.35f;
//     const float ROW_Z_SPACING = 1.2f;
//     const float FIRST_ROW_Z = 1.0f;
//
//     const float AISLE_W = 1.2f;
//
//     const float TOTAL_SEAT_W = COLS * SEAT_W;
//     const float TOTAL_W = TOTAL_SEAT_W + 2.0f * AISLE_W;
//
//     const float STEP_OVERLAP = 0.1f; // uklanja pukotine
//
//     // --- Ray picking (center crosshair) ---
//     glm::vec3 rayO = cameraPos;
//     glm::vec3 rayD = glm::normalize(cameraFront);
//
//     // prag: koliko blizu rayu mora da bude centar sedista
//     const float PICK_RADIUS = 0.45f;
//
//     int bestR = -1, bestC = -1;
//     float bestT = 1e9f; // najmanja projekcija napred (closest along ray)
//
//     //best seat for highlight
//     for (int r = 0; r < ROWS; r++)
//     {
//         float y = r * STEP_H;
//         float z = FIRST_ROW_Z + r * ROW_Z_SPACING;
//
//         for (int c = 0; c < COLS; c++)
//         {
//             float x = (c - (COLS - 1) * 0.5f) * SEAT_W;
//
//             glm::vec3 seatCenter = {x, y, z};
//
//             glm::vec3 v = seatCenter - rayO;
//             float t = glm::dot(v, rayD);
//             if (t <= 0.0f) continue;
//
//             glm::vec3 closest = rayO + rayD * t;
//             float distToRay = glm::length(seatCenter - closest);
//
//             if (distToRay <= PICK_RADIUS && t < bestT)
//             {
//                 bestT = t;
//                 bestR = r;
//                 bestC = c;
//             }
//         }
//     }
//
//     for (int r = 0; r < ROWS; r++)
//     {
//         float y = r * STEP_H;
//         float z = FIRST_ROW_Z + r * ROW_Z_SPACING;
//
//         // steps underneath the seats
//         if (r > 0)
//         {
//             if (r == ROWS - 1) {
//                 renderer.drawCube(
//                     {0.0f, y - 0.15f, z + 0.7f},
//                     {TOTAL_W, STEP_H, 2.4f + STEP_OVERLAP},
//                     {0.15f, 0.15f, 0.15f}
//                 );
//             }
//             else
//             {
//                 renderer.drawCube(
//                     {0.0f, y - 0.15f, z},
//                     {TOTAL_W, STEP_H, ROW_Z_SPACING + STEP_OVERLAP},
//                     {0.15f, 0.15f, 0.15f}
//                 );
//             }
//         }
//
//         for (int c = 0; c < COLS; c++)
//         {
//             float x = (c - (COLS - 1) * 0.5f) * SEAT_W;
//
//             glm::vec3 seatPos = {x, y, z};
//
//             bool highlighted = (r == bestR && c == bestC);
//
//             glm::vec3 seatColor = highlighted
//                 ? glm::vec3(0.9f, 0.4f, 0.2f)
//                 : glm::vec3(0.6f, 0.1f, 0.1f);
//
//             renderer.drawCube(
//                 seatPos,
//                 {SEAT_W * 0.9f, SEAT_H, SEAT_D},
//                 seatColor
//             );
//         }
//     }
// }

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
    float slideZ = doorOpen * 1.2f;

    renderer.drawCube(
        {-3.9f, 0.9f, -1.2f - slideZ},
        {0.1f, 1.8f, 0.9f},
        doorColor
    );

    renderer.drawCube(
        {3.9f, 0.9f, -1.2f - slideZ},
        {0.1f, 1.8f, 0.9f},
        doorColor
    );
}

