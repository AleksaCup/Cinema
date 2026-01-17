#include "Simulation.h"
#include "../Header/Util.h"
#include <cstdlib>
#include <cmath>

Simulation::Simulation(SeatGrid* grid, RectRenderer* rr)
    : state(State::Idle), grid(grid), rectRenderer(rr)
{
    personTex = loadImageToTexture("../Resources/person.png");
    personRenderer.init();
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
    int bought   = grid->getBoughtCount();

    int maxPeople = reserved + bought;
    if (maxPeople <= 0)
    {
        people.clear();
        return;
    }

    // random br ljudi
    int count = 1 + rand() % maxPeople;

    // rezervisana ili kupljena
    std::vector<Seat*> candidates;
    for (auto& s : grid->seats)
        if (s.status != SeatStatus::Free)
            candidates.push_back(&s);

    people.clear();
    for (int i = 0; i < count; i++)
    {
        Seat* target = candidates[rand() % candidates.size()];

        // ulaz: (-0.9, 0.9)
        people.emplace_back(-0.9f, 0.9f, target);
    }
}

void Simulation::update(float dt)
{
    switch (state)
    {
        case State::OpeningDoors:
            state = State::PeopleEntering;
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
            if (movieTimer > 20.0f)  // 20 sekundi
                state = State::PeopleLeaving;
            break;

        case State::PeopleLeaving:
        {
            for (auto& p : people)
            {
                if (p.hasExited) continue;

                float doorX = -0.9f;
                float doorY =  0.9f;

                float dx = doorX - p.x;
                float dy = doorY - p.y;

                if (std::fabs(dx) > 0.01f)
                {
                    // flipuju se naopkao
                    p.flipY = true;
                    p.x += (dx > 0 ? p.speed : -p.speed) * dt;
                }
                else if (std::fabs(dy) > 0.01f)
                {
                    // flipuju se normalno
                    p.flipY = false;
                    p.y += (dy > 0 ? p.speed : -p.speed) * dt;
                }
                else
                {
                    // stigao do vrata
                    p.hasExited = true;
                }
            }

            if (allLeft())
                state = State::ClosingDoors;
            break;
        }

        case State::ClosingDoors:
            people.clear();
            movieTimer = 0.0f;
            colorTimer = 0.0f;
            // vracanje u idle stanje
            grid->resetSeats();
            state = State::Idle;
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

    // svakih 20 frejmova na 75 fps
    if (colorTimer > (1.0f / 75.0f) * 20.0f)
    {
        colorTimer = 0.0f;
        r = (float)(rand() % 100) / 100.0f;
        g = (float)(rand() % 100) / 100.0f;
        b = (float)(rand() % 100) / 100.0f;
    }
}

void Simulation::draw()
{
    // overlay preko celog ekrana dok je idle
    if (state == State::Idle)
        rectRenderer->draw(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.5f);

    // platno
    float rr = (state == State::MoviePlaying ? r : 1.0f);
    float gg = (state == State::MoviePlaying ? g : 1.0f);
    float bb = (state == State::MoviePlaying ? b : 1.0f);

    rectRenderer->draw(0.0f, 0.9f, 0.8f, 0.05f, rr, gg, bb, 1.0f);

    // vrata
    if (state == State::OpeningDoors || state == State::PeopleEntering || state == State::PeopleLeaving)
    {
        rectRenderer->draw(-0.9f, 0.9f, 0.08f, 0.15f, 0.4f, 0.2f, 0.1f, 1.0f);
    }

    // ljudi
    for (auto& p : people)
        personRenderer.draw(p.x, p.y, 0.08f, personTex, p.flipY);
}
