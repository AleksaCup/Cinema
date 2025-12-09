#pragma once
#include "Seat.h"
#include <vector>

class SeatGrid {
public:
    int rows;
    int cols;
    float seatSize;
    std::vector<Seat> seats;

    SeatGrid(int rows, int cols, float seatSize);

    void draw(unsigned int shader, unsigned int vao);

    Seat* getSeatAt(float mouseX, float mouseY);
    void toggleSeat(float mouseX, float mouseY);

    std::vector<Seat*> findContiguousFreeSeats(int count);
    void markBought(const std::vector<Seat*>& seatsToBuy);

    int getReservedCount();
    int getBoughtCount();

    void resetSeats();

};
