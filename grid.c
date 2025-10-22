#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "constants.h"
#include "position.h"
#include "animal.h"

#include "grid.h"

struct Grid_t {
    int height;
    int width;
    Animal ***animals;  // Tableau 2D de pointeurs vers Animal
    bool **grass;       // Tableau 2D pour l'herbe
};

int gridGetWidth(Grid *grid) {
    return grid->width;
}

int gridGetHeight(Grid *grid) {
    return grid->height;
}

Grid *gridCreateEmpty(int height, int width) {
    Grid *grid = malloc(sizeof(Grid));
    if (!grid) return NULL;
    
    grid->height = height;
    grid->width = width;
    
    // Allouer le tableau d'animaux
    grid->animals = malloc(height * sizeof(Animal**));
    grid->grass = malloc(height * sizeof(bool*));
    
    for (int i = 0; i < height; i++) {
        grid->animals[i] = malloc(width * sizeof(Animal*));
        grid->grass[i] = malloc(width * sizeof(bool));
        
        for (int j = 0; j < width; j++) {
            grid->animals[i][j] = NULL;
            grid->grass[i][j] = false;
        }
    }
    
    return grid;
}

void gridFree(Grid *grid) {
    if (!grid) return;
    
    for (int i = 0; i < grid->height; i++) {
        // Libérer les animaux
        for (int j = 0; j < grid->width; j++) {
            if (grid->animals[i][j]) {
                animalDie(grid->animals[i][j]);
            }
        }
        free(grid->animals[i]);
        free(grid->grass[i]);
    }
    
    free(grid->animals);
    free(grid->grass);
    free(grid);
}

void gridAddGrass(Grid *grid, Position pos) {
    if(gridCellIsEmpty(grid,pos))
    grid->grass[pos.row][pos.col] = true; // transformer la case en herbe
}

void gridAddAnimal(Grid *grid, Animal *animal, Position pos) {
    if (gridCellIsOutside(grid, pos)) return;
    
    grid->animals[pos.row][pos.col] = animal;
    grid->grass[pos.row][pos.col] = false;  // retirer si y'avait de l'herbe 
    return;
}

void gridMoveAnimal(Grid *grid, Position pos, Position new_pos) {
    if (gridCellIsOutside(grid, pos) || gridCellIsOutside(grid, new_pos)) return;
    
    Animal *animal = grid->animals[pos.row][pos.col];
    if (!animal) return;
    
    // Si la destination contient un animal (lapin mangé par loup), le supprimer
    if (grid->animals[new_pos.row][new_pos.col] != NULL) {
        animalDie(grid->animals[new_pos.row][new_pos.col]); // il est ded le lapinou
    }
    
    // Déplacer l'animal
    grid->animals[new_pos.row][new_pos.col] = animal;
    grid->animals[pos.row][pos.col] = NULL;
    
    // juste le loup détruit l'herbe en se déplaçant
    const char *animalName = animalGetName(animal);
    if (strcmp(animalName, "Wolf") == 0) {
        grid->grass[new_pos.row][new_pos.col] = false;  // enlever l'herbe seulement pour les loups
    }
}

void gridMakeEmpty(Grid *grid, Position pos) {
    if (gridCellIsOutside(grid, pos)) return;
    
    if (grid->animals[pos.row][pos.col]) { // tuer l'animal sur la case -> mettre null 
        animalDie(grid->animals[pos.row][pos.col]);
        grid->animals[pos.row][pos.col] = NULL;
    }

    grid->grass[pos.row][pos.col] = false;
}

bool gridCellIsOutside(Grid *grid, Position pos) {
    return (pos.row < 0 || pos.row >= grid->height || 
            pos.col < 0 || pos.col >= grid->width);
}

bool gridCellIsEmpty(Grid *grid, Position pos) {
    if (gridCellIsOutside(grid, pos)) return false;
    
    return (grid->animals[pos.row][pos.col] == NULL && 
            grid->grass[pos.row][pos.col] == false);
}

bool gridCellIsGrass(Grid *grid, Position pos) {
    if (gridCellIsOutside(grid, pos)) return false;
    
    return (grid->grass[pos.row][pos.col]);
}

bool gridCellIsAnimal(Grid *grid, Position pos) {
    if (gridCellIsOutside(grid, pos)) return false;
    
    return (grid->animals[pos.row][pos.col] != NULL);
}

Animal *gridGetAnimal(Grid *grid, Position pos) {
    if (gridCellIsOutside(grid, pos)) return NULL;
    
    return grid->animals[pos.row][pos.col];
}

static int calculateDistance(Position a, Position b) {
    return abs(a.row - b.row) + abs(a.col - b.col);
}
int gridFindClosestAnimal(Grid *grid, Position pos, int maxDistance, const char *name) {
    int minDist = maxDistance + 1;
    
    // Parcourir uniquement la zone autour de pos dans le rayon maxDistance
    int rowMin = (pos.row - maxDistance < 0) ? 0 : pos.row - maxDistance;
    int rowMax = (pos.row + maxDistance >= grid->height) ? grid->height - 1 : pos.row + maxDistance;
    int colMin = (pos.col - maxDistance < 0) ? 0 : pos.col - maxDistance;
    int colMax = (pos.col + maxDistance >= grid->width) ? grid->width - 1 : pos.col + maxDistance;
    
    for (int i = rowMin; i <= rowMax; i++) {
        for (int j = colMin; j <= colMax; j++) {
            Animal *animal = grid->animals[i][j];
            if (animal && strcmp(animalGetName(animal), name) == 0) {
                Position animalPos = {i, j};
                int dist = calculateDistance(pos, animalPos);
                
                if (dist <= maxDistance && dist < minDist) {
                    minDist = dist;
                    if (minDist == 0) return 0;  
                }
            }
        }
    }
    
    return minDist;
}

int gridFindClosestGrass(Grid *grid, Position pos, int maxDistance) {
    int minDist = maxDistance + 1;
    
    // Parcourir uniquement la zone autour de pos dans le rayon maxDistance
    int rowMin = (pos.row - maxDistance < 0) ? 0 : pos.row - maxDistance;
    int rowMax = (pos.row + maxDistance >= grid->height) ? grid->height - 1 : pos.row + maxDistance;
    int colMin = (pos.col - maxDistance < 0) ? 0 : pos.col - maxDistance;
    int colMax = (pos.col + maxDistance >= grid->width) ? grid->width - 1 : pos.col + maxDistance;
    
    for (int i = rowMin; i <= rowMax; i++) {
        for (int j = colMin; j <= colMax; j++) {
            if (grid->grass[i][j]) {
                Position grassPos = {i, j};
                int dist = calculateDistance(pos, grassPos);
                
                if (dist <= maxDistance && dist < minDist) {
                    minDist = dist;
                    if (minDist == 0) return 0;  // Optimisation
                }
            }
        }
    }
    
    return minDist;
}

