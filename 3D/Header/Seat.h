#ifndef CINEMA_SEAT_H
#define CINEMA_SEAT_H

#include <glm/glm.hpp>

enum class SeatState {
    FREE,
    RESERVED,
    BOUGHT,
    OCCUPIED
};

struct Seat {
    int row;
    int col;
    glm::vec3 position;
    SeatState state;
    
    Seat() : row(0), col(0), position(0.0f), state(SeatState::FREE) {}
    
    Seat(int r, int c, glm::vec3 pos)
        : row(r), col(c), position(pos), state(SeatState::FREE) {}
    
    void toggle() {
        if (state == SeatState::FREE)
            state = SeatState::RESERVED;
        else if (state == SeatState::RESERVED)
            state = SeatState::FREE;
    }
    
    void markBought() {
        state = SeatState::BOUGHT;
    }
    
    void markOccupied() {
        state = SeatState::OCCUPIED;
    }
    
    bool isFree() const {
        return state == SeatState::FREE;
    }
    
    glm::vec3 getColor() const {
        switch (state) {
            case SeatState::FREE: return glm::vec3(0.2f, 0.8f, 0.2f); // Green
            case SeatState::RESERVED: return glm::vec3(1.0f, 1.0f, 0.0f); // Yellow
            case SeatState::BOUGHT: return glm::vec3(0.8f, 0.2f, 0.2f); // Red
            case SeatState::OCCUPIED: return glm::vec3(0.5f, 0.5f, 0.5f); // Gray
            default: return glm::vec3(1.0f, 1.0f, 1.0f); // White
        }
    }
};

#endif