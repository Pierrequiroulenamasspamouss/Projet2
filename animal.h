#ifndef ANIMAL_H
#define ANIMAL_H

#include <stdbool.h>

#include "action.h"

/**
 * @brief Opaque declaration of the animal.
 *
 */
typedef struct Animal_t Animal;

// We can not include grid.h, so we declare the grid structure.
struct Grid_t;

/**
 * @brief Create a new animal.
 * 
 * @param name the name, for instance "rabbit" for rabbits.
 * @param priority the closer to 0, the quicker the animal.
 * @param energy the initial energy of the animal.
 * @param actionEnergy the energy lost by the animal at each step.
 * @param eatEnergy the energy gained by the animal when it eats.
 * @param findAction a function that finds the best action given the grid.
 * @param reproduce a function that creates a new animal if the animal has enough energy.
 * @return Animal* 
 */
Animal *animalCreate(
    const char *name,
    unsigned int priority,
    int energy,
    int actionEnergy,
    int eatEnergy,
    Action(*findAction)(Animal *, struct Grid_t *, Position),
    Animal *(*reproduce)(Animal *));

/**
 * @brief Free an animal.
 * 
 * @param animal the animal to free.
 */
void animalDie(Animal *animal);

/**
 * @brief Return the name of an animal.
 * 
 * @param animal the animal.
 * @return const char*
 */
const char *animalGetName(Animal *animal);

/**
 * @brief Return the priority of an animal.
 * 
 * @param animal the animal.
 * @return unsigned int 
 */
unsigned int animalGetPriority(Animal *animal);

/**
 * @brief Find the best action of an animal given the grid and its position.
 * 
 * @param animal the animal.
 * @param grid the grid.
 * @param pos the current position of the animal.
 * @return Action 
 */
Action animalFindAction(Animal *animal, struct Grid_t *grid, Position pos);

/**
 * @brief Make the animal lose some energy.
 * 
 * @param animal the animal.
 */
void animalLoseEnergy(Animal *animal);

/**
 * @brief Make the animal gain some energy when it eats.
 * 
 * @param animal the animal.
 */
void animalEat(Animal *animal);

/**
 * @brief Reproduce the animal if it has enough energy, or return NULL.
 * 
 * @param animal the parent.
 * @return Animal* the child if the parent has enough energy, or NULL.
 */
Animal *animalReproduce(Animal *animal);

/**
 * @brief Return the energy of an animal.
 *
 * @param animal the animal.
 * @return int
 */
int animalGetEnergy(Animal *animal);

/**
 * @brief Set the energy of an animal.
 * 
 * @param animal the animal.
 * @param energy the energy.
 */
void animalSetEnergy(Animal *animal, int energy);

#endif
