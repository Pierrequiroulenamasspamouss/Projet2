#ifndef GRID_H
#define GRID_H

#include <stdbool.h>

#include "position.h"
#include "animal.h"

/**
 * @brief Opaque declaration of the grid.
 *
 */
typedef struct Grid_t Grid;

/**
 * @brief Create an empty grid.
 *
 * @param height the height.
 * @param width the width.
 * @return Grid*
 */
Grid *gridCreateEmpty(int height, int width);

/**
 * @brief Get the width of a grid.
 *
 * @param grid the grid.
 * @return int
 */
int gridGetWidth(Grid *grid);

/**
 * @brief Get the height of a grid.
 *
 * @param grid the grid.
 * @return int
 */
int gridGetHeight(Grid *grid);

/**
 * @brief Free a grid.
 *
 * @param grid the grid.
 */
void gridFree(Grid *grid);

/**
 * @brief Empty a cell. If an animal is there, it is killed.
 *
 * @param grid the grid.
 * @param pos the position of the cell inside the grid.
 */
void gridMakeEmpty(Grid *grid, Position pos);

/**
 * @brief Add grass in a cell. If an animal is there, nothing is done.
 *
 * @param grid the grid.
 * @param pos the position of the cell inside the grid.
 */
void gridAddGrass(Grid *grid, Position pos);

/**
 * @brief Add an animal in a cell. If the cell is not empty, nothing is done.
 *
 * @param grid the grid.
 * @param animal the animal to add.
 * @param pos the position of the cell.
 */
void gridAddAnimal(Grid *grid, Animal *animal, Position pos);

/**
 * @brief Move an animal from a cell to an other. If the original cell does not contain
 * an animal, or if the new cell contains an animal, nothing is done.
 *
 * @param grid the grid.
 * @param pos the position of the initial cell.
 * @param new_pos the position of the new cell.
 */
void gridMoveAnimal(Grid *grid, Position pos, Position new_pos);

/**
 * @brief Return whether a cell is outside of the grid.
 *
 * @param grid the grid.
 * @param pos the position of the cell.
 * @return true
 * @return false
 */
bool gridCellIsOutside(Grid *grid, Position pos);

/**
 * @brief Return whether a cell is empty.
 *
 * @param grid the grid.
 * @param pos the position of the cell.
 * @return true
 * @return false
 */
bool gridCellIsEmpty(Grid *grid, Position pos);

/**
 * @brief Return whether a cell contains grass.
 *
 * @param grid the grid.
 * @param pos the position of the cell.
 * @return true
 * @return false
 */
bool gridCellIsGrass(Grid *grid, Position pos);

/**
 * @brief Return whether a cell contains an animal.
 *
 * @param grid the grid.
 * @param pos the position of the cell.
 * @return true
 * @return false
 */
bool gridCellIsAnimal(Grid *grid, Position pos);

/**
 * @brief Return the animal contained in a cell. If the cell does not contain an animal,
 * NULL is returned.
 *
 * @param grid the grid.
 * @param pos the position of the cell.
 * @return Animal*
 */
Animal *gridGetAnimal(Grid *grid, Position pos);

/**
 * @brief Find the closest animal with a given name from a cell and returns the distance
 * between it and the cell. If no animal is found within a distance equal to
 * maxDistance, then maxDistance + 1 is returned.
 *
 * Note that the distance 0 has to be considered.
 *
 * @param grid the grid.
 * @param pos the position of the cell.
 * @param maxDistance the maximum distance to look for the animal.
 * @param name the name of the animal.
 * @return int
 */
int gridFindClosestAnimal(Grid *grid, Position pos, int maxDistance, const char *name);

/**
 * @brief Find the closest grass from a cell and returns the distance between it and the
 * cell. If no grass is found within a distance equal to maxDistance, then
 * maxDistance + 1 is returned.
 *
 * Note that the distance 0 has to be considered.
 *
 * @param grid the grid.
 * @param pos the position of the cell.
 * @param maxDistance the maximum distance to look for the empty cell.
 * @return int
 */
int gridFindClosestGrass(Grid *grid, Position pos, int maxDistance);

#endif
