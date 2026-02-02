#ifndef CINEMA_SEAT_H
#define CINEMA_SEAT_H

#include <glm/glm.hpp>

enum class SeatState {
    FREE,
    RESERVED,
    BOUGHT,
    OCCUPIED
};

class Seat {
private:
    int row;
    int col;
    glm::vec3 position;
    SeatState state;

public:
    Seat(int r, int c, const glm::vec3& pos);

    // Getters
    const glm::vec3& getPosition() const;
    SeatState getState() const;

    // State logic
    void toggle();
    void markBought();
    void markOccupied();

    // Visual
    glm::vec3 getColor() const;
};

#endif // CINEMA_SEAT_H
