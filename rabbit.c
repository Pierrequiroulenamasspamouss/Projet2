#include <stdlib.h>
#include <limits.h>
#include "animal.h"
#include "grid.h"
#include "constants.h"
#include "position.h"
#include "action.h"
#include "rabbit.h"
#include "stdio.h"

static int calculateRabbitScore(Grid *grid, Position current, Position target, int maxDist)
{
    // verif si la case est en dehors de la grille
    if (gridCellIsOutside(grid, target))
    {
        return INT_MIN;
    }
    
    // sinon si la case contient deja un autre animal
    if (gridCellIsAnimal(grid, target))
    {
        if (target.row != current.row || target.col != current.col)
        {
            return INT_MIN;
        }
    }
    
    // check distances
    int dg = gridFindClosestGrass(grid, target, maxDist);
    int dw = gridFindClosestAnimal(grid, target, maxDist, wolfName);
    
    // score = -wg*dg + ww*dw
    return -rabbitGrassWeight * dg + rabbitWolfWeight * dw;
}

static Action rabbitFindAction(Animal *rabbit, Grid *grid, Position pos)
{
    // 5 mouvements possibles
    Move moves[5] = {
        {0, 0},  // paboujé
        {-1, 0}, // ho
        {1, 0},  // ba
        {0, -1}, // gosh
        {0, 1}   // drwat
    };
    
    int bestScore = INT_MIN;
    Move bestMoves[5]; // array de taille 5 pour max 5 bons mouvements
    int bestCount = 0;
    
    // regarder toutes les positions possibles
    for (int m = 0; m < 5; m++)
    {
        Position newPos = {pos.row + moves[m].drow, pos.col + moves[m].dcol};
        int score = calculateRabbitScore(grid, pos, newPos, rabbitMaxDistance);
        
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
    
    // paboujer si aucun mouvement valide
    if (bestCount == 0)
    {
        Action action = {false, moves[0]};
        return action;
    }
    
    // au pif parmi les mouvements qu'il peut faire
    Move chosenMove = bestMoves[rand() % bestCount];
    Position finalPos = {pos.row + chosenMove.drow, pos.col + chosenMove.dcol};
    
    // check si on peut manger de l'herbe a position finale
    bool canEat = gridCellIsGrass(grid, finalPos);
    
    Action action = {canEat, chosenMove};
    return action;
}

static Animal *rabbitReproduce(Animal *rabbit)
{
    // juste reproduire si seuil depasse
    if (animalGetEnergy(rabbit) > rabbitReproduceThreshold)
    {
        animalSetEnergy(rabbit, animalGetEnergy(rabbit) - rabbitReproduceEnergy);
        return rabbitCreate();
    }
    return NULL;
}

Animal *rabbitCreate(void)
{
    return animalCreate(
        rabbitName,
        rabbitPriority,
        rabbitInitEnergy,
        rabbitActionEnergy,
        rabbitEatEnergy,
        rabbitFindAction,
        rabbitReproduce);
}
