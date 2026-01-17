#ifndef CINEMA_SEATGRID_H
#define CINEMA_SEATGRID_H

#include <vector>
#include <glm/glm.hpp>
#include "Seat3D.h"
#include "Renderers/CubeRenderer.h"

class SeatGrid3D {
private:
    int rows;
    int cols;
    std::vector<std::vector<Seat3D>> seats;

    float seatWidth = 0.4f;
    float seatHeight = 0.5f;
    float seatDepth = 0.4f;
    float seatSpacing = 0.1f;
    float rowSpacing = 0.8f;
    float stepHeight = 0.2f; // Height increase per row

public:
    SeatGrid3D(int numRows, int numCols);

    void draw(CubeRenderer& renderer);

    Seat3D* getSeat(int row, int col);

    std::vector<Seat3D*> findContiguousFreeSeats(int count);
    void markBought(const std::vector<Seat3D*>& seats);

    // Ray casting for seat picking
    Seat3D* rayCastPick(const glm::vec3& rayOrigin, const glm::vec3& rayDirection);

    // Getters
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    std::vector<std::vector<Seat3D>>& getSeats() { return seats; }

private:
    bool rayIntersectsBox(const glm::vec3& rayOrigin,
                          const glm::vec3& rayDirection,
                          const glm::vec3& boxMin,
                          const glm::vec3& boxMax,
                          float& distance);
};

#endif //CINEMA_SEATGRID_H