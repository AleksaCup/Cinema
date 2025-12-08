#pragma once
#include <GLFW/glfw3.h>

enum class SeatStatus {
    Free,
    Reserved,
    Bought
};

class Seat {
public:
    float x, y;
    float size;
    SeatStatus status;

    Seat(float x, float y, float size);

    bool contains(float mx, float my) const;
    void toggleReserve();  // promena izmedju slobodno i rezervisano
    void setBought();

    void draw(unsigned int shader, unsigned int vao);
};
