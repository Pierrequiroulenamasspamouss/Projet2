#ifndef SIMULATION_GIF_H
#define SIMULATION_GIF_H

#include "simulation.h"
#include "gifenc.h"

typedef ge_GIF SimGIF;

SimGIF *createGif(Simulation *s, const char *filename, int pixelSize);

void addFrame(Simulation *s, ge_GIF *gif, uint16_t delay);

void closeGif(ge_GIF *gif);

#endif
