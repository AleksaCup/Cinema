#ifndef SIMULATION_H
#define SIMULATION_H

#include "SeatGrid.h"
#include "Renderers/CubeRenderer.h"
#include <vector>

#include "Person.h"

class Simulation
{
public:
    float doorOpen;
    enum class State
    {
        Idle,
        OpeningDoors,
        PeopleEntering,
        MoviePlaying,
        PeopleLeaving,
        ClosingDoors
    };

    Simulation(SeatGrid* grid, CubeRenderer* renderer);

    void start();
    void update(float dt);
    void draw();

    State getState() const;
    bool isMoviePlaying() const;
    float getDoorOpen() const;

private:
    State state;
    SeatGrid* grid;
    CubeRenderer* cubeRenderer;

    std::vector<Person> people;

    float movieTimer;
    float colorTimer;
    float r, g, b;

    void spawnPeople();
    bool allSeated();
    bool allLeft();
    void updateMovie(float dt);
    int getCurrentScreenIndex() const;
};

#endif