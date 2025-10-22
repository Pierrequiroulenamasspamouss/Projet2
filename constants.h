#ifndef CONSTANTS_H
#define CONSTANTS_H

// The names of the animals
extern const char *rabbitName;
extern const char *wolfName;

// The priorities of the animals, the lower the faster.
extern const unsigned int wolfPriority;
extern const unsigned int rabbitPriority;

// The maximum distances the animals can see. Used when computing the scores.
extern const int rabbitMaxDistance;
extern const int wolfMaxDistance;

// Energy parameters related to the rabbit.

// Energy when created.
extern const int rabbitInitEnergy;
// Energy consumed when an action is performed.
extern const int rabbitActionEnergy;
// Energy gained when grass is eated.
extern const int rabbitEatEnergy;
// Energy lost when the rabbit reproduces.
extern const int rabbitReproduceEnergy;
// Energy threshold required for reproduction.
extern const int rabbitReproduceThreshold;

// Energy parameters related to the wolf.

// Energy when created.
extern const int wolfInitEnergy;
// Energy consumed when an action is performed.
extern const int wolfActionEnergy;
// Energy gained when grass is eated.
extern const int wolfEatEnergy;
// Energy lost when the wolf reproduces.
extern const int wolfReproduceEnergy;
// Energy threshold required for reproduction.
extern const int wolfReproduceThreshold;

// Weights used when computing the score of the rabbit.
extern const int rabbitGrassWeight;
extern const int rabbitWolfWeight;

// Probabilites used when making appear grass.
// The probability that some grass will appear on an empty cell is:
// P = grassProb + N * grassIncProb, where N is the number of adjacent cells with grass
// (0 <= N <= 4)
extern const double grassProb;
extern const double grassIncProb;

#endif
