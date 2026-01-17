#pragma once
#include "Renderers/PersonRenderer.h"
#include "Renderers/RectRenderer.h"
#include "SeatGrid.h"
#include "Person.h"
#include "State.h"

#include <vector>

class Simulation
{
public:
    State state;
    SeatGrid* grid;
    RectRenderer* rectRenderer;
    std::vector<Person> people;
    float movieTimer = 0.0f;
    float colorTimer = 0.0f;
    float r=1, g=1, b=1;

    unsigned int personTex;
    PersonRenderer personRenderer;

    Simulation(SeatGrid* grid, RectRenderer* rr);

    void start();
    void update(float dt);
    void draw();

private:
    void spawnPeople();
    bool allSeated();
    bool allLeft();
    void updateMovie(float dt);
};
