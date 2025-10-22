#include <stdlib.h>
#include <stdbool.h>

#include "position.h"
#include "action.h"
#include "constants.h"
#include "grid.h"

#include "animal.h"

struct Animal_t
{
    int energy;
    const char *name;
    unsigned int priority;

    int actionEnergy;
    int eatEnergy;

    Action (*findAction)(Animal *, Grid *, Position);
    Animal *(*reproduce)(Animal *);
};

Animal *animalCreate(
    const char *name,
    unsigned int priority,
    int energy,
    int actionEnergy,
    int eatEnergy,
    Action (*findAction)(Animal *, Grid *, Position),
    Animal *(*reproduce)(Animal *))
{
    Animal *animal = malloc(sizeof(Animal));

    animal->name = name;
    animal->priority = priority;
    animal->energy = energy;
    animal->actionEnergy = actionEnergy;
    animal->eatEnergy = eatEnergy;

    animal->findAction = findAction;
    animal->reproduce = reproduce;

    return animal;
}

void animalDie(Animal *animal)
{
    free(animal);
}

const char *animalGetName(Animal *animal)
{
    return animal->name;
}

unsigned int animalGetPriority(Animal *animal)
{
    return animal->priority;
}

Action animalFindAction(Animal *animal, Grid *grid, Position pos)
{
    return animal->findAction(animal, grid, pos);
}

void animalLoseEnergy(Animal *animal)
{
    animal->energy -= animal->actionEnergy;
}
void animalEat(Animal *animal)
{
    animal->energy += animal->eatEnergy;
}

Animal *animalReproduce(Animal *animal)
{
    return animal->reproduce(animal);
}

int animalGetEnergy(Animal *animal)
{
    return animal->energy;
}

void animalSetEnergy(Animal *animal, int energy)
{
    animal->energy = energy;
}
