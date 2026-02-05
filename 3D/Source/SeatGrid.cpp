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
    const float AISLE_W = 1.2f;
    const float TOTAL_SEAT_W = cols * seatW;
    const float TOTAL_W = TOTAL_SEAT_W + 2.0f * AISLE_W;
    const float STEP_OVERLAP = 0.1f;

    for (int r = 0; r < rows; r++)
    {
        float y = r * stepHeight;
        float z = firstRowZ + r * rowSpacing;

        // ===== STEP / PLATFORM =====
        if (r > 0)
        {
            bool lastRow = (r == rows - 1);

            renderer.drawCube(
                lastRow
                    ? glm::vec3(0.0f, y - 0.15f, z + 0.7f)
                    : glm::vec3(0.0f, y - 0.15f, z),
                lastRow
                    ? glm::vec3(TOTAL_W, stepHeight, 2.4f + STEP_OVERLAP)
                    : glm::vec3(TOTAL_W, stepHeight, rowSpacing + STEP_OVERLAP),
                glm::vec3(0.15f)
            );
        }

        // ===== SEATS (TEXTURED) =====
        for (int c = 0; c < cols; c++)
        {
            Seat& seat = seats[r][c];

            unsigned int tex = seat.getTexture();

            glm::vec3 tint =
                (&seat == highlightedSeat)
                ? glm::vec3(1.25f)   // highlight
                : glm::vec3(1.0f);

            renderer.drawCubeTextured(
                seat.getPosition(),
                glm::vec3(seatW * 0.9f, seatH, seatD),
                tex,
                tint
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

void SeatGrid::updateHighlight(const glm::vec3& rayOrigin, const glm::vec3& rayDir)
{
    highlightedSeat = pickSeat(rayOrigin, rayDir);
}

std::vector<Seat*> SeatGrid::findContiguousFreeSeats(int count)
{
    if (count <= 0) return {};
    for (int r = rows - 1; r >= 0; --r)
    {
        std::vector<Seat*> block;
        block.reserve(count);

        for (int c = cols - 1; c >= 0; --c)
        {
            Seat& s = seats[r][c];

            if (s.isFree())
            {
                block.push_back(&s);
                if ((int)block.size() == count)
                    return block;
            }
            else
            {
                block.clear();
            }
        }
    }

    return {};
}

void SeatGrid::markBought(const std::vector<Seat*>& seatsToBuy)
{
    for (Seat* s : seatsToBuy)
        if (s) s->markBought();
}

// ===== NOVO: Funkcije za simulaciju =====

int SeatGrid::getReservedCount() const
{
    int count = 0;
    for (const auto& row : seats)
        for (const auto& seat : row)
            if (seat.getState() == SeatState::RESERVED)
                count++;
    return count;
}

int SeatGrid::getBoughtCount() const
{
    int count = 0;
    for (const auto& row : seats)
        for (const auto& seat : row)
            if (seat.getState() == SeatState::BOUGHT)
                count++;
    return count;
}

void SeatGrid::resetSeats()
{
    for (auto& row : seats)
        for (auto& seat : row)
            seat.reset();
}

int SeatGrid::getRows() const
{
    return rows;
}

int SeatGrid::getCols() const
{
    return cols;
}

// ===== RAY INTERSECTION =====

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