#include "SeatGrid.h"
#include <limits>
#include <algorithm>

SeatGrid::SeatGrid(int r, int c)
    : rows(r), cols(c)
{
    seats.resize(rows);

    for (int row = 0; row < rows; row++) {
        float y = row * stepHeight;
        float z = firstRowZ + row * rowSpacing;

        for (int col = 0; col < cols; col++) {
            float x = (col - (cols - 1) * 0.5f) * seatW;
            glm::vec3 pos(x, y, z);

            seats[row].emplace_back(row, col, pos);
        }
    }
}

void SeatGrid::draw(CubeRenderer& renderer)
{
    for (auto& row : seats) {
        for (auto& seat : row) {
            renderer.drawCube(
                seat.getPosition(),
                { seatW * 0.9f, seatH, seatD },
                seat.getColor()
            );
        }
    }
}

Seat* SeatGrid::getSeat(int row, int col)
{
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return nullptr;
    return &seats[row][col];
}

Seat* SeatGrid::pickSeat(const glm::vec3& rayOrigin,
                          const glm::vec3& rayDir)
{
    Seat* picked = nullptr;
    float bestT = std::numeric_limits<float>::max();

    for (auto& row : seats) {
        for (auto& seat : row) {
            glm::vec3 half(seatW * 0.45f, seatH * 0.5f, seatD * 0.45f);
            glm::vec3 minB = seat.getPosition() - half;
            glm::vec3 maxB = seat.getPosition() + half;

            float t;
            if (rayIntersectsBox(rayOrigin, rayDir, minB, maxB, t)) {
                if (t < bestT) {
                    bestT = t;
                    picked = &seat;
                }
            }
        }
    }
    return picked;
}

bool SeatGrid::rayIntersectsBox(const glm::vec3& ro, const glm::vec3& rd, const glm::vec3& minB, const glm::vec3& maxB, float& t)
{
    float tmin = (minB.x - ro.x) / rd.x;
    float tmax = (maxB.x - ro.x) / rd.x;
    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (minB.y - ro.y) / rd.y;
    float tymax = (maxB.y - ro.y) / rd.y;
    if (tymin > tymax) std::swap(tymin, tymax);

    if (tmin > tymax || tymin > tmax) return false;

    tmin = std::max(tmin, tymin);
    tmax = std::min(tmax, tymax);

    float tzmin = (minB.z - ro.z) / rd.z;
    float tzmax = (maxB.z - ro.z) / rd.z;
    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if (tmin > tzmax || tzmin > tmax) return false;

    t = tmin;
    return t > 0.0f;
}
