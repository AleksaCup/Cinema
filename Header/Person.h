#pragma once
#include "../Header/Seat.h"

class Person {
public:
    float x, y;
    float speed;
    Seat* targetSeat;
    bool seated;
    bool hasExited = false;
    bool flipY = false;

    Person(float startX, float startY, Seat* seat);

    void update(float deltaTime);
};
