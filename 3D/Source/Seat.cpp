#include "Seat.h"

unsigned int Seat::texFree = 0;
unsigned int Seat::texReserved = 0;
unsigned int Seat::texBought = 0;

Seat::Seat(int r, int c, const glm::vec3& pos)
    : row(r), col(c), position(pos), state(SeatState::FREE) {}

const glm::vec3& Seat::getPosition() const {
    return position;
}

SeatState Seat::getState() const {
    return state;
}

void Seat::toggle() {
    if (state == SeatState::FREE)
        state = SeatState::RESERVED;
    else if (state == SeatState::RESERVED)
        state = SeatState::FREE;
}

void Seat::markBought() {
    state = SeatState::BOUGHT;
}

// void Seat::markOccupied() {
//     state = SeatState::OCCUPIED;
// }

bool Seat::isFree() const
{
    return state == SeatState::FREE;
}

bool Seat::isBought() const
{
    return state == SeatState::BOUGHT;
}

// NOVO: Reset za simulaciju
void Seat::reset() {
    state = SeatState::FREE;
}

glm::vec3 Seat::getColor() const {
    switch (state) {
        case SeatState::FREE:     return {0.2f, 0.8f, 0.2f}; // green
        case SeatState::RESERVED: return {1.0f, 1.0f, 0.0f}; // yellow
        case SeatState::BOUGHT:   return {0.8f, 0.2f, 0.2f}; // red
            // case SeatState::OCCUPIED: return {0.5f, 0.5f, 0.5f}; // gray
    }
    return {1.0f, 1.0f, 1.0f};
}

unsigned int Seat::getTexture() const {
    switch (state) {
        case SeatState::FREE:     return texFree;
        case SeatState::RESERVED: return texReserved;
        case SeatState::BOUGHT:   return texBought;
    }
    return texFree;
}