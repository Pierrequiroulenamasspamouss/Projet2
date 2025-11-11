#include <stdlib.h>
#include <limits.h>
#include "animal.h"
#include "grid.h"
#include "constants.h"
#include "position.h"
#include "action.h"
#include "wolf.h"
#include "string.h"

static int calculateWolfScore(Grid *grid, Position current, Position target, int maxDist)
{
    // si la case est en dehors de la grille
    if (gridCellIsOutside(grid, target))
    {
        return INT_MIN;
    }

    // si la case contient un autre loup
    if (gridCellIsAnimal(grid, target))
    {
        Animal *occupant = gridGetAnimal(grid, target);
        
        if (occupant && strcmp(animalGetName(occupant), wolfName) == 0)
        {
            if (target.row != current.row || target.col != current.col)
            {
                return INT_MIN;
            }
        }
    }

    // distance au lapin le plus proche
    int dr = gridFindClosestAnimal(grid, target, maxDist, rabbitName);
    return -dr; // -dr car on veut trouver la distance minimale -> plus proche = moins negatif
}

static Action wolfFindAction(Animal *wolf, Grid *grid, Position pos)
{
    (void)wolf; // on se sert pas de son état
    // Les 13 mouvements possibles
    Move moves[13] = {
        {0, 0},   // pas bouger
        {-1, 0},  // haut 1
        {1, 0},   // bas 1
        {0, -1},  // gauche 1
        {0, 1},   // droite 1
        {-2, 0},  // haut 2
        {2, 0},   // bas 2
        {0, -2},  // gauche 2
        {0, 2},   // droite 2
        {-1, -1}, // haut-gauche
        {-1, 1},  // haut-droite
        {1, -1},  // bas-gauche
        {1, 1}    // bas-droite
    };

    int bestScore = INT_MIN;
    Move bestMoves[13];
    int bestCount = 0;

    // tester toutes les positions possibles
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

    // pas de mouvement valide -> pas bouger
    if (bestCount == 0)
    {
        Action action = {false, moves[0]};
        return action;
    }

    // au hasard parmi les meilleurs mouvements
    Move chosenMove = bestMoves[rand() % bestCount];
    Position finalPos = {pos.row + chosenMove.drow, pos.col + chosenMove.dcol};

    // verifier si la nouvelle case permet de bouffer le lapin
    bool canEat = false;
    if (gridCellIsAnimal(grid, finalPos))
    {
        Animal *target = gridGetAnimal(grid, finalPos);
        if (target && strcmp(animalGetName(target), rabbitName) == 0)
        {
            canEat = true;
        }
    }

    Action action = {canEat, chosenMove};
    return action;
}

static Animal *wolfReproduce(Animal *wolf)
{
    // meme idee que pour rabbitReproduce -> energie minimale necessaire
    if (animalGetEnergy(wolf) >= wolfReproduceThreshold)
    {
        animalSetEnergy(wolf, animalGetEnergy(wolf) - wolfReproduceEnergy);
        return wolfCreate();
    }
    return NULL;
}

Animal *wolfCreate(void)
{
    // meme idee que pour rabbitCreate
    return animalCreate(
        wolfName,
        wolfPriority,
        wolfInitEnergy,
        wolfActionEnergy,
        wolfEatEnergy,
        wolfFindAction,
        wolfReproduce);
}