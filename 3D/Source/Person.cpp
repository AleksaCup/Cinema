#include "Person.h"
#include "Seat.h"

#include <cmath>
#include <glm/glm.hpp>

Person::Person(float startX, float startY, float startZ, Seat* targetSeat, int appearance)
    : x(startX), y(startY), z(startZ), target(targetSeat),
      seated(false), hasExited(false), speed(1.5f), stage(0), appearanceIndex(appearance)
{
    //left or right entrance
    isLeftSide = (startX < 0.0f);
}

void Person::update(float dt)
{
    if (seated || !target) return;

    //stage 0: go diagonally through the coridor to your row
    //stage 1: go horizontally to your seat

    glm::vec3 targetPos = target->getPosition();

    if (stage == 0)
    {
        float aisleX = isLeftSide ? -3.5f : 3.5f;
        float rowZ = targetPos.z;
        float rowY = targetPos.y + 0.1f;

        float dx = aisleX - x;
        float dy = rowY - y;
        float dz = rowZ - z;
        float dist = std::sqrt(dx*dx + dy*dy + dz*dz);

        if (dist > 0.1f)
        {
            x += (dx / dist) * speed * dt;
            y += (dy / dist) * speed * dt;
            z += (dz / dist) * speed * dt;
        }
        else
        {
            stage = 1;
        }
    }
    else if (stage == 1)
    {
        float dx = targetPos.x - x;

        if (std::fabs(dx) > 0.1f)
        {
            x += (dx > 0 ? speed : -speed) * dt;
        }
        else
        {
            x = targetPos.x;
            y = targetPos.y + 0.1f;
            z = targetPos.z;
            seated = true;
        }
    }
}