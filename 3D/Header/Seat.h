#ifndef CINEMA_SEAT_H
#define CINEMA_SEAT_H

#include <glm/glm.hpp>

enum class SeatState {
    FREE,
    RESERVED,
    BOUGHT,
};

class Seat {
private:
    int row;
    int col;
    glm::vec3 position;
    SeatState state;

public:
    static unsigned int texFree;
    static unsigned int texReserved;
    static unsigned int texBought;
    Seat(int r, int c, const glm::vec3& pos);

    // Getters
    const glm::vec3& getPosition() const;
    SeatState getState() const;

    // State logic
    void toggle();
    void markBought();
    bool isFree() const;
    bool isBought() const;
    // void markOccupied();

    void reset();

    // Visual
    glm::vec3 getColor() const;
    unsigned int getTexture() const;
};

#endif // CINEMA_SEAT_H
