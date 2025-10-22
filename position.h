#ifndef POSITION_H
#define POSITION_H

/**
 * @brief Structure to represent the position of a cell in the grid.
 * The position {row=0, col=0} corresponds to the top-left corner of the grid, and
 * the position {row=height-1, col=width-1} corresponds to the bottom-right corner.
 *
 */
typedef struct
{
    int row;
    int col;
} Position;

/**
 * @brief Structure to represent a relative move from a position.
 * For instance, the move {drow=1, dcol=-1} means going down one row and going left one
 * column.
 *
 */
typedef struct
{
    int drow;
    int dcol;
} Move;

#endif
