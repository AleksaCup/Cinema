#include "Person.h"
#include <cmath>

Person::Person(float startX, float startY, Seat* seat)
    : x(startX), y(startY), speed(0.4f),
      targetSeat(seat), seated(false)
{}

void Person::update(float dt)
{
    // 1) Prvo idemo VERTIKALNO do reda
    float dy = targetSeat->y - y;

    if (fabs(dy) > 0.01f)
    {
        // Normalan orijent (nije okrenut)
        flipY = false;

        y += (dy > 0 ? speed : -speed) * dt;
        return;
    }

    // 2) Kada smo stigli u red → idemo HORIZONTALNO do sedišta
    float dx = targetSeat->x - x;

    if (fabs(dx) > 0.01f)
    {
        // sada osoba treba da bude naopako okrenuta
        flipY = true;

        x += (dx > 0 ? speed : -speed) * dt;
        return;
    }

    // 3) Kada je na svom mestu — sedi
    seated = true;
}