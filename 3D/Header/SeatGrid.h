#ifndef CINEMA_SEATGRID_H
#define CINEMA_SEATGRID_H

#include <vector>
#include <glm/glm.hpp>
#include "Seat.h"
#include "Renderers/CubeRenderer.h"

class SeatGrid {
private:
    int rows;
    int cols;

    std::vector<std::vector<Seat>> seats;

    // === DIMENSIONS ===
    float seatW = 0.55f;
    float seatH = 0.6f;
    float seatD = 0.55f;

    float rowSpacing = 1.2f;
    float stepHeight = 0.35f;
    float firstRowZ = 1.0f;

public:
    SeatGrid(int rows, int cols);

    void draw(CubeRenderer& renderer);

    Seat* getSeat(int row, int col);

    // Ray picking (crosshair / mouse)
    Seat* pickSeat(const glm::vec3& rayOrigin,
                   const glm::vec3& rayDir);

private:
    bool rayIntersectsBox(const glm::vec3& rayOrigin,
                          const glm::vec3& rayDir,
                          const glm::vec3& boxMin,
                          const glm::vec3& boxMax,
                          float& t);
};

#endif
