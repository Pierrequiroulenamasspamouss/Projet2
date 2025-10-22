#include "simulation_gif.h"

#include <stdio.h>
#include <string.h>

#include "simulation.h"
#include "grid.h"
#include "gifenc.h"
#include "position.h"
#include "constants.h"

static int _pixelSize;

SimGIF *createGif(Simulation *s, const char *filename, int pixelSize)
{
    _pixelSize = pixelSize;
    Grid *g = simulationGetGrid(s);

    ge_GIF *gif = ge_new_gif(
        filename,
        gridGetWidth(g) * _pixelSize,
        gridGetHeight(g) * _pixelSize,
        (uint8_t[]){
            /* palette */
            0xDE, 0xB8, 0x87, /* 0 -> brown */
            0x00, 0x00, 0x00, /* 1 -> black */
            0xFF, 0xFF, 0xFF, /* 2 -> white */
            0x2E, 0xB3, 0x37, /* 3 -> green */
            0xBF, 0x38, 0x2E, /* 4 -> red */
            0x00, 0x00, 0x00,
            0x00, 0x00, 0x00,
            0x00, 0x00, 0x00,
        },
        3, /* palette depth == log2(# of colors) */
        1  /* Play it once */
    );
    return gif;
}

void addFrame(Simulation *s, ge_GIF *gif, uint16_t delay)
{
    int ind = 0;
    uint8_t color;
    Grid *g = simulationGetGrid(s);
    int height = gridGetHeight(g);
    int width = gridGetWidth(g);

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            Position pos;
            pos.row = h;
            pos.col = w;
            const char *name;
            if (gridCellIsEmpty(g, pos))
            {
                color = 0;
            }
            else if (gridCellIsGrass(g, pos))
            {
                color = 3;
            }
            else
            {
                name = animalGetName(gridGetAnimal(g, pos));

                if (name == NULL)
                {
                    color = 0;
                }
                else if (strcmp(name, rabbitName) == 0)
                {
                    color = 2;
                }
                else if (strcmp(name, wolfName) == 0)
                {
                    color = 1;
                }
                else
                {
                    color = 4;
                }
            }
            for (int i = 0; i < _pixelSize; i++)
            {
                for (int j = 0; j < _pixelSize; j++)
                {
                    gif->frame[ind + i * width * _pixelSize + j] = color;
                }
            }

            ind += _pixelSize;
        }

        ind += (_pixelSize - 1) * _pixelSize * width;
    }

    ge_add_frame(gif, delay);
}

void closeGif(ge_GIF *gif)
{
    ge_close_gif(gif);
}
