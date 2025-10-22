#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdlib.h>

#include "position.h"
#include "action.h"
#include "grid.h"

/**
 * @brief Opaque structure for the simulation.
 *
 */
typedef struct Simulation_t Simulation;

/**
 * @brief Create a simulation with an empty grid.
 *
 * @param height the height.
 * @param width the width.
 * @return Simulation*
 */
Simulation *simulationCreate(int height, int width);

/**
 * @brief Free a simulation and the associated grid.
 *
 * @param sim the simulation.
 */
void simulationFree(Simulation *sim);

/**
 * @brief Add grass in a cell.
 *
 * @param sim the simulation.
 * @param pos the position of the cell.
 */
void simulationAddGrass(Simulation *sim, Position pos);

/**
 * @brief Add an animal in a cell.
 *
 * @param sim the simulation.
 * @param animal the animal.
 * @param pos the position of the cell.
 */
void simulationAddAnimal(Simulation *sim, Animal *animal, Position pos);

/**
 * @brief Do a simulation step, i.e. each animal performs an action as described in the
 * statement, then grass randomly appears.
 *
 * @param sim the simulation.
 */
void simulationStep(Simulation *sim);

/**
 * @brief Count the number of animals of a certain type.
 *
 * @param sim the simulation.
 * @param name the name of the animal.
 * @return int
 */
int simulationCountAnimals(Simulation *sim, char *name);

/**
 * @brief Count the number of grass.
 *
 * @param sim the simulation.
 * @return int
 */
int simulationCountGrass(Simulation *sim);

/**
 * @brief Get the grid.
 *
 * @param sim the simulation.
 * @return Grid*
 */
Grid *simulationGetGrid(Simulation *sim);

#endif
