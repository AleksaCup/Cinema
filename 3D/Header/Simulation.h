#ifndef SIMULATION_H
#define SIMULATION_H

#include "SeatGrid.h"
#include "Renderers/CubeRenderer.h"
#include <vector>

// Forward declaration
class Seat;

// Person class
class Person
{
public:
    float x, y, z;
    Seat* target;
    bool seated;
    bool hasExited;
    float speed;
    int stage;  // 0=dijagonalno do reda, 1=horizontalno do sedišta
    bool isLeftSide;  // Da li ulazi sa leve strane
    int appearanceIndex;

    Person(float startX, float startY, float startZ, Seat* targetSeat, int appearance);
    void update(float dt);
};

// Simulation class
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
    float r, g, b;  // Screen colors during movie

    void spawnPeople();
    bool allSeated();
    bool allLeft();
    void updateMovie(float dt);
    int getCurrentScreenIndex() const;
};

#endif // SIMULATION_H