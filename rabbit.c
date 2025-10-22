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
    // Si la case est en dehors de la grille
    if (gridCellIsOutside(grid, target))
    {
        return INT_MIN;
    }
    
    // Si la case contient un animal (autre que le lapin lui-même)
    if (gridCellIsAnimal(grid, target))
    {
        if (target.row != current.row || target.col != current.col)
        {
            return INT_MIN;
        }
    }
    
    // Calculer distances
    int dg = gridFindClosestGrass(grid, target, maxDist);
    int dw = gridFindClosestAnimal(grid, target, maxDist, wolfName);
    
    // Score = -wg*dg + ww*dw
    return -rabbitGrassWeight * dg + rabbitWolfWeight * dw;
}

static Action rabbitFindAction(Animal *rabbit, Grid *grid, Position pos)
{
    // 5 mouvements possibles (sur place, gauche, droite, haut, bas)
    Move moves[5] = {
        {0, 0},  // sur place
        {-1, 0}, // haut
        {1, 0},  // bas
        {0, -1}, // gauche
        {0, 1}   // droite
    };
    
    int bestScore = INT_MIN;
    Move bestMoves[5];
    int bestCount = 0;
    
    // Parcourir toutes les positions possibles
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
    
    // Si aucun mouvement valide, rester sur place
    if (bestCount == 0)
    {
        Action action = {false, moves[0]};
        return action;
    }
    
    // Choisir aléatoirement parmi les meilleurs mouvements
    Move chosenMove = bestMoves[rand() % bestCount];
    Position finalPos = {pos.row + chosenMove.drow, pos.col + chosenMove.dcol};
    
    // Vérifier si on peut manger de l'herbe à la position finale
    bool canEat = gridCellIsGrass(grid, finalPos);
    
    Action action = {canEat, chosenMove};
    return action;
}

static Animal *rabbitReproduce(Animal *rabbit)
{
    // La reproduction se fait si l'énergie DÉPASSE le seuil
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
