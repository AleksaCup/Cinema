#include "Simulation.h"
#include <cstdlib>
#include <cmath>
#include <glm/glm.hpp>

#include "Textures/TextureManager.h"
#include "Renderers/PersonRenderer.h"

Simulation::Simulation(SeatGrid* grid, CubeRenderer* renderer)
    : state(State::Idle), grid(grid), cubeRenderer(renderer)
{
    movieTimer = 0.0f;
    colorTimer = 0.0f;
    r = g = b = 1.0f;
    doorOpen = 0.0f;
}

void Simulation::start()
{
    if (state != State::Idle) return;

    state = State::OpeningDoors;
    spawnPeople();
}

void Simulation::spawnPeople()
{
    int reserved = grid->getReservedCount();
    int bought = grid->getBoughtCount();

    int maxPeople = reserved + bought;
    if (maxPeople <= 0)
    {
        people.clear();
        return;
    }

    //number of people
    int count = 1 + rand() % maxPeople;

    //candidates(reserved + bought seats)
    std::vector<Seat*> candidates;
    for (int r = 0; r < grid->getRows(); r++)
    {
        for (int c = 0; c < grid->getCols(); c++)
        {
            Seat* s = grid->getSeat(r, c);
            if (s && s->getState() != SeatState::FREE)
                candidates.push_back(s);
        }
    }

    people.clear();
    for (int i = 0; i < count; i++)
    {
        Seat* target = candidates[rand() % candidates.size()];

        float doorX = (i % 2 == 0) ? -3.5f : 3.5f;  //left and right doors
        float doorZ = -1.0f;
        float doorY = 0.05f;

        int appearance = 1 + (rand() % 15);
        people.emplace_back(doorX, doorY, doorZ, target, appearance);
    }
}

void Simulation::update(float dt)
{
    switch (state)
    {
        case State::OpeningDoors:
            doorOpen += dt;
            if (doorOpen >= 1.0f) {
                doorOpen = 1.0f;
                state = State::PeopleEntering;
            }
            break;

        case State::PeopleEntering:
        {
            for (auto& p : people)
                if (!p.seated)
                    p.update(dt);

            if (!people.empty() && allSeated())
            {
                movieTimer = 0.0f;
                colorTimer = 0.0f;
                state = State::MoviePlaying;
            }
            break;
        }

        case State::MoviePlaying:
            updateMovie(dt);
            if (movieTimer > 20.0f)  //duration of movie
                state = State::PeopleLeaving;
            break;

        case State::PeopleLeaving:
        {
            for (auto& p : people)
            {
                if (p.hasExited) continue;

                //stage 0: go to the coridor
                //stage 1: go down diagonally

                if (p.stage == 1)
                {
                    p.stage = 0;
                    p.seated = false;
                }

                if (p.stage == 0)
                {
                    float aisleX = p.isLeftSide ? -3.5f : 3.5f;
                    float dx = aisleX - p.x;

                    if (std::fabs(dx) > 0.1f)
                    {
                        p.x += (dx > 0 ? p.speed : -p.speed) * dt;
                    }
                    else
                    {
                        p.stage = -1;  //diagonal movement to the doors
                    }
                }
                else if (p.stage == -1)
                {
                    //diagonal movement to the doors
                    float doorX = p.isLeftSide ? -3.5f : 3.5f;
                    float doorY = 0.3f;
                    float doorZ = -1.0f;

                    float dx = doorX - p.x;
                    float dy = doorY - p.y;
                    float dz = doorZ - p.z;

                    float dist = std::sqrt(dx*dx + dy*dy + dz*dz);

                    if (dist > 0.05f)
                    {
                        //diagonal movement to the doors
                        p.x += (dx / dist) * p.speed * dt;
                        p.y += (dy / dist) * p.speed * dt;
                        p.z += (dz / dist) * p.speed * dt;
                    }
                    else
                    {
                        //reached the doors
                        p.hasExited = true;
                    }
                }
            }

            if (allLeft())
                state = State::ClosingDoors;
            break;
        }

        case State::ClosingDoors:
            doorOpen -= dt;
            if (doorOpen <= 0.0f) {
                doorOpen = 0.0f;
                people.clear();
                movieTimer = 0.0f;
                colorTimer = 0.0f;
                grid->resetSeats();
                state = State::Idle;
            }
            break;

        default:
            break;
    }
}

bool Simulation::allSeated()
{
    for (auto& p : people)
        if (!p.seated)
            return false;
    return true;
}

bool Simulation::allLeft()
{
    for (auto& p : people)
        if (!p.hasExited)
            return false;
    return true;
}

void Simulation::updateMovie(float dt)
{
    movieTimer += dt;
    colorTimer += dt;

    if (colorTimer > (1.0f / 75.0f) * 20.0f)
    {
        colorTimer = 0.0f;
        r = (float)(rand() % 100) / 100.0f;
        g = (float)(rand() % 100) / 100.0f;
        b = (float)(rand() % 100) / 100.0f;
    }
}

int Simulation::getCurrentScreenIndex() const
{
    if (state != State::MoviePlaying)
        return 1;

    int index = static_cast<int>(movieTimer) + 1;

    if (index < 1) index = 1;
    if (index > 20) index = 20;

    return index;
}


void Simulation::draw()
{
    //people
    for (auto& p : people)
    {
        unsigned int personTex = TextureManager::get("person" + std::to_string(p.appearanceIndex));
        PersonRenderer::draw(*cubeRenderer, glm::vec3(p.x, p.y, p.z), personTex);
    }

    //SCREEN TEXTURE DURING SIMULATION
    if (state != State::Idle)
    {
        int idx = (state == State::MoviePlaying) ? getCurrentScreenIndex() : 1;

        unsigned int tex =
            TextureManager::get("screen" + std::to_string(idx));

        cubeRenderer->drawCubeTextured(
            {0.0f, 2.6f, -1.80f},
            {6.6f, 3.6f, 0.02f},
            tex,
            {1.0f, 1.0f, 1.0f}
        );
    }

}


Simulation::State Simulation::getState() const {
    return state;
}

bool Simulation::isMoviePlaying() const
{
    return state == State::MoviePlaying;
}

float Simulation::getDoorOpen() const
{
    return doorOpen;
}