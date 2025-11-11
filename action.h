#ifndef ACTION_H
#define ACTION_H

#include <stdbool.h>

#include "position.h"

/**
 * @brief Structure that represents an action of an animal.
 * 
 * - move gives the movement that the animal wants to do.
 * 
 * - eat says whether or not the animal eats what is at its new position. 
 */
typedef struct Action_t
{
    bool eat;
    Move move;
} Action;

#endif
