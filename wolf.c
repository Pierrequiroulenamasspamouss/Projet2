#include <stdlib.h>
#include <limits.h> // Need for INT_MIN

#include "animal.h"
#include "grid.h"
#include "constants.h"
#include "position.h"
#include "action.h"

#include "wolf.h"
#include "string.h"
static int calculateWolfScore(Grid *grid, Position current, Position target, int maxDist)
{
    // Si la case est en dehors de la grille
    if (gridCellIsOutside(grid, target))
    {
        return INT_MIN;
    }

    // Si la case contient un autre loup (pas le loup actuel)
    if (gridCellIsAnimal(grid, target))
    {
        Animal *occupant = gridGetAnimal(grid, target);
        // CORRECTION : utiliser strcmp au lieu de ==
        if (occupant && strcmp(animalGetName(occupant), wolfName) == 0)
        {
            if (target.row != current.row || target.col != current.col)
            {
                return INT_MIN;
            }
        }
    }

    // Calculer distance au lapin le plus proche
    int dr = gridFindClosestAnimal(grid, target, maxDist, rabbitName);

    // Score = -dr (on veut minimiser la distance)
    return -dr;
}

static Action wolfFindAction(Animal *wolf, Grid *grid, Position pos)
{
    // Les 13 mouvements possibles
    Move moves[13] = {
        {0, 0},   // paboujer
        {-1, 0},  // ho 1
        {1, 0},   // ba 1
        {0, -1},  // gosh 1
        {0, 1},   // drwat 1
        {-2, 0},  // ho 2
        {2, 0},   // ba 2
        {0, -2},  // gosh 2
        {0, 2},   // drwat 2
        {-1, -1}, // haut-gauche
        {-1, 1},  // haut-droite
        {1, -1},  // bas-gauche
        {1, 1}    // bas-droite
    };

    int bestScore = INT_MIN;
    Move bestMoves[13];
    int bestCount = 0;

    // tester toutes les pos possibles
    for (int m = 0; m < 13; m++)
    {
        Position newPos = {pos.row + moves[m].drow, pos.col + moves[m].dcol};

        int score = calculateWolfScore(grid, pos, newPos, wolfMaxDistance);

        if (score > bestScore)
        {
            bestScore = score;
            bestMoves[0] = moves[m];
            bestCount = 1;
        }
        else if (score == bestScore && score != INT_MIN)
        {
            bestMoves[bestCount++] = moves[m];
        }
    }

    // pas de mouvement valide -> paboujer
    if (bestCount == 0)
    {
        Action action = {false, moves[0]};
        return action;
    }

    // Choisir aléatoirement parmi les meilleurs mouvements
    Move chosenMove = bestMoves[rand() % bestCount];
    Position finalPos = {pos.row + chosenMove.drow, pos.col + chosenMove.dcol};

        // Vérifier si on peut manger un lapin seulement si l'énergie n'est pas pleine
    bool canEat = false;
    if (animalGetEnergy(wolf) < wolfReproduceThreshold) {
        if (gridCellIsAnimal(grid, finalPos))
        {
            Animal *target = gridGetAnimal(grid, finalPos);
            if (target && strcmp(animalGetName(target), rabbitName) == 0)
            {
                canEat = true;
            }
        }
    }

    Action action = {canEat, chosenMove};
    return action;
}

static Animal *wolfReproduce(Animal *wolf)
{
    if (animalGetEnergy(wolf) >= wolfReproduceThreshold)
    {
        animalSetEnergy(wolf, animalGetEnergy(wolf) - wolfReproduceEnergy);
        return wolfCreate();
    }
    return NULL;
}
Animal *wolfCreate(void)
{
    return animalCreate(
        wolfName,
        wolfPriority,
        wolfInitEnergy,
        wolfActionEnergy,
        wolfEatEnergy,
        wolfFindAction,
        wolfReproduce);
}