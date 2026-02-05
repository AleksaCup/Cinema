#ifndef PERSON_H
#define PERSON_H

class Seat;

class Person
{
public:
    float x, y, z;
    Seat* target;
    bool seated;
    bool hasExited;
    float speed;
    int stage;
    bool isLeftSide;
    int appearanceIndex;

    Person(float startX, float startY, float startZ, Seat* targetSeat, int appearance);
    void update(float dt);
};

#endif
