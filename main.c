#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "simulation.h"
#include "grid.h"
#include "simulation_gif.h"
#include "rabbit.h"
#include "wolf.h"

struct Config
{
    int grid_dim;
    int num_steps;
    double wolf_prob;
    double rabbit_prob;
    double grass_prob;
    const char *gif_file;
};

static void printUsageAndExit(int value);
static struct Config parseArg(int argc, char const *argv[]);

static void printUsageAndExit(int value)
{
    const char usage[] = ("Usage: ./simulate -s <grid_dim> -t <num_steps> -w <wolf_prob> "
                          "-r <rabbit_prob> -g <grass_prob> -f <gif_file>\n"
                          "- grid_dim: height and width of the grid\n"
                          "- num_steps: number of steps of simulation\n"
                          "- wolf_prob: probability of having a wolf in a cell at initialization\n"
                          "- rabbit_prob: probability of having a rabbit in a cell at initialization\n"
                          "- grass_prob: probability of having a grass in a cell at initialization\n"
                          "- gif_file: filename to store the gif\n");

    fprintf(stderr, usage);
    exit(value);
}

static struct Config parseArg(int argc, char const *argv[])
{
    struct Config config = {
        100,
        100,
        0.003,
        0.03,
        0.1,
        NULL,
    };

    if ((argc >= 2) && (strncmp(argv[1], "-h", 2) == 0))
    {
        printUsageAndExit(0);
    }

    int ind = 1;
    const char *option;
    while ((ind + 1) < argc)
    {
        option = argv[ind];

        if (strncmp(option, "-s", 2) == 0)
        {
            int grid_dim = atoi(argv[ind + 1]);
            if (grid_dim <= 0)
            {
                fprintf(stderr, "Error: invalid grid dimension, got '%s'.\n", argv[ind + 1]);
                printUsageAndExit(1);
            }

            config.grid_dim = grid_dim;
        }

        else if (strncmp(option, "-t", 2) == 0)
        {
            int num_steps = atoi(argv[ind + 1]);
            if (num_steps <= 0)
            {
                fprintf(stderr, "Error: invalid number of steps, got '%s'.\n", argv[ind + 1]);
                printUsageAndExit(1);
            }

            config.num_steps = num_steps;
        }

        else if (strncmp(option, "-w", 2) == 0)
        {
            char *endptr;
            double wolf_prob = strtod(argv[ind + 1], &endptr);
            if ((endptr[0] != '\0') || wolf_prob < 0)
            {
                fprintf(stderr, "Error: invalid wolf prob, got '%s'.\n", argv[ind + 1]);
                printUsageAndExit(1);
            }

            config.wolf_prob = wolf_prob;
        }

        else if (strncmp(option, "-r", 2) == 0)
        {
            char *endptr;
            double rabbit_prob = strtod(argv[ind + 1], &endptr);
            if ((endptr[0] != '\0') || rabbit_prob < 0)
            {
                fprintf(stderr, "Error: invalid rabbit prob, got '%s'.\n", argv[ind + 1]);
                printUsageAndExit(1);
            }

            config.rabbit_prob = rabbit_prob;
        }

        else if (strncmp(option, "-g", 2) == 0)
        {
            char *endptr;
            double grass_prob = strtod(argv[ind + 1], &endptr);
            if ((endptr[0] != '\0') || grass_prob < 0)
            {
                fprintf(stderr, "Error: invalid grass prob, got '%s'.\n", argv[ind + 1]);
                printUsageAndExit(1);
            }

            config.grass_prob = grass_prob;
        }

        else if (strncmp(option, "-f", 2) == 0)
        {
            config.gif_file = argv[ind + 1];
        }

        else
        {
            fprintf(stderr, "Unknown option: %s.\n", option);
            printUsageAndExit(1);
        }

        ind += 2;
    }

    if (ind != argc)
    {
        fprintf(stderr, "Incorrect number of arguments.\n");
        printUsageAndExit(1);
    }

    if ((config.wolf_prob + config.rabbit_prob + config.grass_prob) > 1.0)
    {
        fprintf(stderr, "Error: Sum of the 3 probabilities is above 1.\n");
        printUsageAndExit(1);
    }

    return config;
}

int main(int argc, char const *argv[])
{

    struct Config config = parseArg(argc, argv);

    printf("Config:\n");
    printf("grid_dim: %d\n", config.grid_dim);
    printf("num_steps: %d\n", config.num_steps);
    printf("wolf_prob: %lf\n", config.wolf_prob);
    printf("rabbit_prob: %lf\n", config.rabbit_prob);
    printf("grass_prob: %lf\n", config.grass_prob);
    printf("gif_file: %s\n", config.gif_file);
    printf("\n");

    const int pixelSize = 10;

    srand(time(NULL));

    // Create simulation
    Simulation *sim = simulationCreate(config.grid_dim, config.grid_dim);

    // Randomly fill grid
    for (int r = 0; r < config.grid_dim; r++)
    {
        for (int c = 0; c < config.grid_dim; c++)
        {
            double random = ((double)rand()) / ((double)RAND_MAX);

            if (random < config.wolf_prob)
            {
                simulationAddAnimal(sim, wolfCreate(), (Position){r, c});
            }

            else if (random < config.wolf_prob + config.rabbit_prob)
            {
                simulationAddAnimal(sim, rabbitCreate(), (Position){r, c});
            }

            else if (random < config.wolf_prob + config.rabbit_prob + config.grass_prob)
            {
                simulationAddGrass(sim, (Position){r, c});
            }
        }
    }

    printf("Step,Rabbits,Wolfs,Grass\n");

    printf("0,%d,%d,%d\n",
           simulationCountAnimals(sim, "Rabbit"),
           simulationCountAnimals(sim, "Wolf"),
           simulationCountGrass(sim));

    SimGIF *gif = NULL;
    if (config.gif_file != NULL)
    {
        gif = createGif(sim, config.gif_file, pixelSize);
        addFrame(sim, gif, 10);
    }

    for (int i = 0; i < config.num_steps; i++)
    {
        simulationStep(sim);

        int numRabbits = simulationCountAnimals(sim, "Rabbit");
        int numWolfs = simulationCountAnimals(sim, "Wolf");
        int numGrass = simulationCountGrass(sim);

        fprintf(stdout, "%d,%d,%d,%d\n",
                i + 1,
                numRabbits,
                numWolfs,
                numGrass);
        fflush(stdout);

        if (gif != NULL)
        {
            addFrame(sim, gif, 10);
        }

        if ((numGrass + numRabbits + numWolfs) == 0)
            break;

        if (numGrass == (config.grid_dim * config.grid_dim))
            break;
    }

    if (gif != NULL)
    {
        closeGif(gif);
    }

    simulationFree(sim);

    return 0;
}
