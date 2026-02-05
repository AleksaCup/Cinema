#ifndef SEATGRID_H
#define SEATGRID_H

#include "Seat.h"
#include "Renderers/CubeRenderer.h"
#include <vector>
#include <glm/glm.hpp>

class SeatGrid
{
public:
    SeatGrid(int rows, int cols);

    void draw(CubeRenderer& renderer);

    Seat* getSeat(int row, int col);
    Seat* getHighlightedSeat() { return highlightedSeat; }

    void updateHighlight(const glm::vec3& rayOrigin, const glm::vec3& rayDir);

    std::vector<Seat*> findContiguousFreeSeats(int count);
    void markBought(const std::vector<Seat*>& seatsToBuy);

    int getReservedCount() const;
    int getBoughtCount() const;
    void resetSeats();
    int getRows() const;
    int getCols() const;

private:
    int rows;
    int cols;

    std::vector<std::vector<Seat>> seats;
    Seat* highlightedSeat = nullptr;

    //Seat dimensions
    const float seatW = 0.6f;
    const float seatH = 0.4f;
    const float seatD = 0.5f;

    //Row spacing
    const float rowSpacing = 1.0f;
    const float firstRowZ = 1.5f;
    const float stepHeight = 0.3f;

    Seat* pickSeat(const glm::vec3& rayOrigin, const glm::vec3& rayDir);
    bool rayIntersectsBox(const glm::vec3& ro, const glm::vec3& rd,
                          const glm::vec3& minB, const glm::vec3& maxB, float& t);
};

#endif