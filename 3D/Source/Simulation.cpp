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

    // Random broj ljudi
    int count = 1 + rand() % maxPeople;

    // Rezervisana ili kupljena sedišta
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

        // Ulaz: polovina ide sa leve strane, polovina sa desne
        float doorX = (i % 2 == 0) ? -3.5f : 3.5f;  // Leva i desna vrata
        float doorZ = -1.0f;  // Pozicija vrata u Z osi
        float doorY = 0.3f;   // Visina vrata (malo iznad poda)

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
            if (movieTimer > 20.0f)  // 20 sekundi
                state = State::PeopleLeaving;
            break;

        case State::PeopleLeaving:
        {
            for (auto& p : people)
            {
                if (p.hasExited) continue;

                // Stage 0: Izađi iz reda do hodnika (horizontalno do kraja reda)
                // Stage 1: Idi dijagonalno do vrata

                if (p.stage == 1)  // Još je u stage 1 (sedeo)
                {
                    // Vrati se na stage 0 za izlazak
                    p.stage = 0;
                    p.seated = false;
                }

                if (p.stage == 0)
                {
                    // Prvo izađi horizontalno do kraja hodnika
                    float aisleX = p.isLeftSide ? -3.5f : 3.5f;
                    float dx = aisleX - p.x;

                    if (std::fabs(dx) > 0.1f)
                    {
                        p.x += (dx > 0 ? p.speed : -p.speed) * dt;
                    }
                    else
                    {
                        // Stigao do hodnika, sad dijagonalno ka vratima
                        p.stage = -1;  // Oznaka za dijagonalno kretanje ka vratima
                    }
                }
                else if (p.stage == -1)
                {
                    // Dijagonalno kretanje do vrata
                    float doorX = p.isLeftSide ? -3.5f : 3.5f;
                    float doorY = 0.3f;
                    float doorZ = -1.0f;

                    float dx = doorX - p.x;
                    float dy = doorY - p.y;
                    float dz = doorZ - p.z;

                    float dist = std::sqrt(dx*dx + dy*dy + dz*dz);

                    if (dist > 0.05f)
                    {
                        // Dijagonalno kretanje ka vratima
                        p.x += (dx / dist) * p.speed * dt;
                        p.y += (dy / dist) * p.speed * dt;
                        p.z += (dz / dist) * p.speed * dt;
                    }
                    else
                    {
                        // Stigao do vrata
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
                // Vraćanje u idle stanje
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

    // Svakih 20 frejmova na 75 fps
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
    // ===== PEOPLE =====
    for (auto& p : people)
    {
        unsigned int personTex = TextureManager::get("person" + std::to_string(p.appearanceIndex));
        PersonRenderer::draw(*cubeRenderer, glm::vec3(p.x, p.y, p.z), personTex);
    }

    // ===== SCREEN TEXTURE DURING SIMULATION =====
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
;

bool Simulation::isMoviePlaying() const
{
    return state == State::MoviePlaying;
}

float Simulation::getDoorOpen() const
{
    return doorOpen;
}

// ===== Person Implementation =====

Person::Person(float startX, float startY, float startZ, Seat* targetSeat, int appearance)
    : x(startX), y(startY), z(startZ), target(targetSeat),
      seated(false), hasExited(false), speed(1.5f), stage(0), appearanceIndex(appearance)
{
    // Odrediti sa koje strane ulazi (levo ili desno)
    isLeftSide = (startX < 0.0f);
}

void Person::update(float dt)
{
    if (seated || !target) return;

    // Stage 0: Idi dijagonalno do svog reda (kraj hodnika pored reda)
    // Stage 1: Idi horizontalno do sedišta

    glm::vec3 targetPos = target->getPosition();

    if (stage == 0)
    {
        // Kraj hodnika pored reda - ostaje na strani sa koje je ušao
        float aisleX = isLeftSide ? -3.5f : 3.5f;  // Levi ili desni hodnik
        float rowZ = targetPos.z;  // Z koordinata reda
        float rowY = targetPos.y + 0.5f;  // Y koordinata iznad reda (na nivou osobe koja hoda)

        // Dijagonalno kretanje do kraja hodnika (aisleX, rowY, rowZ)
        float dx = aisleX - x;
        float dy = rowY - y;
        float dz = rowZ - z;
        float dist = std::sqrt(dx*dx + dy*dy + dz*dz);

        if (dist > 0.1f)
        {
            // Krećemo se ka cilju (dijagonalno)
            x += (dx / dist) * speed * dt;
            y += (dy / dist) * speed * dt;
            z += (dz / dist) * speed * dt;
        }
        else
        {
            stage = 1;  // Stigao do kraja hodnika, sada ulazi u red
        }
    }
    else if (stage == 1)
    {
        // Idi horizontalno do sedišta (samo X osa)
        float dx = targetPos.x - x;

        if (std::fabs(dx) > 0.1f)
        {
            x += (dx > 0 ? speed : -speed) * dt;
        }
        else
        {
            // Stigao do sedišta - sedi na sedište (malo iznad sedišta)
            x = targetPos.x;
            y = targetPos.y + 0.5f;  // Na sedištu, ne u sedištu
            z = targetPos.z;
            seated = true;
        }
    }
}