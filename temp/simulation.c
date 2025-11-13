#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "constants.h"
#include "grid.h"
#include "position.h"
#include "action.h"

#include "simulation.h"

struct Simulation_t
{
    Grid *grid;
    int currentStep;
    bool **processed; // tableau des animaux traites
};

Grid *simulationGetGrid(Simulation *sim)
{
    return sim->grid;
}

Simulation *simulationCreate(int height, int width)
{
    Simulation *sim = malloc(sizeof(Simulation));
    if (!sim)
        return NULL;

    sim->grid = gridCreateEmpty(height, width);
    if (!sim->grid)
    {
        free(sim);
        return NULL;
    }

    sim->currentStep = 0;

    sim->processed = malloc(height * sizeof(bool *));
    if (!sim->processed)
    {
        gridFree(sim->grid);
        free(sim);
        return NULL;
    }

    for (int i = 0; i < height; i++)
    {
        sim->processed[i] = calloc(width, sizeof(bool)); // calloc merci google pour initialiser une zone de la mémoire a zero
        if (!sim->processed[i])
        {
            // vider la memoire utilisee
            simulationFree(sim);
            return NULL;
        }
    }

    return sim;
}

void simulationFree(Simulation *sim)
{   
    // on suppose que grid existe et que processed existe aussi, si on veut les free 
    int height = gridGetHeight(sim->grid);
    for (int i = 0; i < height; i++)
    {
        if (sim->processed[i])
        {
            free(sim->processed[i]);
        }
    }
    if (sim->processed)
        free(sim->processed);

    gridFree(sim->grid);
    free(sim);
}

void simulationAddGrass(Simulation *sim, Position pos)
{
    gridAddGrass(sim->grid, pos);
    return;
}

void simulationAddAnimal(Simulation *sim, Animal *animal, Position pos)
{
    gridAddAnimal(sim->grid, animal, pos);
    return;
}

int simulationCountAnimals(Simulation *sim, char *name)
{
    int count = 0;
    int height = gridGetHeight(sim->grid);
    int width = gridGetWidth(sim->grid);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Position pos = {i, j};
            Animal *animal = gridGetAnimal(sim->grid, pos);
            if (animal && strcmp(animalGetName(animal), name) == 0)
            {
                count++;
            }
        }
    }

    return count;
}

int simulationCountGrass(Simulation *sim)
{
    int count = 0;
    int height = gridGetHeight(sim->grid);
    int width = gridGetWidth(sim->grid);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Position pos = {i, j};
            if (gridCellIsGrass(sim->grid, pos))
            {
                count++;
            }
        }
    }

    return count;
}

// fonction privee juste pour compter l'herbe
static int countAdjacentGrass(Grid *grid, Position pos)
{
    int count = 0;
    int height = gridGetHeight(grid);
    int width = gridGetWidth(grid);

    // regarder aux alentours ( dessous, dessus, gauche, droite)
    int drow[] = {-1, 1, 0, 0};
    int dcol[] = {0, 0, -1, 1};

    for (int k = 0; k < 4; k++)
    {
        int ni = pos.row + drow[k];
        int nj = pos.col + dcol[k];

        if (ni >= 0 && ni < height && nj >= 0 && nj < width)
        {
            Position adjPos = {ni, nj};
            if (gridCellIsGrass(grid, adjPos))
            {
                count++;
            }
        }
    }

    return count;
}

void simulationStep(Simulation *sim)
{
    int height = gridGetHeight(sim->grid);
    int width = gridGetWidth(sim->grid);

    // reset le tableau processed pour cette etape
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sim->processed[i][j] = false;
        }
    }

    // on trouve la priorite
    unsigned int maxPriority = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Position pos = {i, j};
            Animal *animal = gridGetAnimal(sim->grid, pos);
            if (animal)
            {
                unsigned int priority = animalGetPriority(animal);
                if (priority > maxPriority)
                {
                    maxPriority = priority;
                }
            }
        }
    }

    for (int p = maxPriority; p >= 0; p--)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                Position pos = {i, j};
                Animal *animal = gridGetAnimal(sim->grid, pos);

                if (animal && animalGetPriority(animal) == (unsigned int)p && !sim->processed[i][j])
                {
                    sim->processed[i][j] = true;

                    if (animalGetEnergy(animal) <= 0)
                    {
                        // l'animal est supprimé
                        gridMakeEmpty(sim->grid, pos);
                        continue;
                    }
                    // trouver l'action a faire
                    Action action = animalFindAction(animal, sim->grid, pos);

                    Position oldPos = pos;
                    // calculer la nouvelle position
                    Position newPos = {
                        pos.row + action.move.drow,
                        pos.col + action.move.dcol};

                    // si le loup veut manger (se deplacer sur un lapin), vider d'abord la case
                    if (action.eat && gridCellIsAnimal(sim->grid, newPos))
                    {
                        Animal *target = gridGetAnimal(sim->grid, newPos);
                        if (target && strcmp(animalGetName(target), rabbitName) == 0)
                        {
                            animalDie(target);                // tuer le lapin
                            gridMakeEmpty(sim->grid, newPos); // vider la case
                        }
                    }

                    // maintenant deplacer l'animal
                    gridMoveAnimal(sim->grid, pos, newPos);
                    sim->processed[newPos.row][newPos.col] = true;
                    animalLoseEnergy(animal); // perdre de l'energie dans tous les cas

                    // bouffer et reproduire
                    if (action.eat)
                    {
                        animalEat(animal);
                        // se reproduire
                        Animal *newborn = animalReproduce(animal);

                        if (newborn)
                        {
                            // verif si oldPos est pas de nouveau utilisee avant de mettre un bebe
                            if (gridGetAnimal(sim->grid, oldPos) == NULL)
                            {
                                gridAddAnimal(sim->grid, newborn, oldPos);
                                sim->processed[oldPos.row][oldPos.col] = true;
                            }
                            else
                            {
                                // tuer le bebe si deja occupe
                                animalDie(newborn);
                            }
                        }
                    }
                }
            }
        }
    }
    // l'herbe pousse
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Position pos = {i, j};

            if (gridGetAnimal(sim->grid, pos) == NULL && !gridCellIsGrass(sim->grid, pos))

            {
                int K = countAdjacentGrass(sim->grid, pos);
                double ph = grassProb + K * grassIncProb;
                if (ph > 1.0)
                    ph = 1.0; // assurer que c'est max 100%
                double r = (double)rand() / RAND_MAX;
                if (r < ph)
                {
                    gridAddGrass(sim->grid, pos);
                }
            }
        }
    }

    sim->currentStep++;
}
