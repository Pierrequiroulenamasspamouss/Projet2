#include "constants.h"

const char *rabbitName = "Rabbit";
const char *wolfName = "Wolf";

const unsigned int wolfPriority = 0;
const unsigned int rabbitPriority = 1;

const int rabbitMaxDistance = 4;
const int wolfMaxDistance = 6;

const int rabbitInitEnergy = 50;
const int rabbitActionEnergy = 5;
const int rabbitEatEnergy = 10;
const int rabbitReproduceEnergy = 25;
const int rabbitReproduceThreshold = 100;

const int wolfInitEnergy = 150;
const int wolfActionEnergy = 5;
const int wolfEatEnergy = 40;
const int wolfReproduceEnergy = 200;
const int wolfReproduceThreshold = 300;

const int rabbitGrassWeight = 1;
const int rabbitWolfWeight = 5;

const double grassProb = 0.0001;
const double grassIncProb = 0.1;
