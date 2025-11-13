#include <stdio.h>
#include "simulation.h"
#include "animal.h"
#include "action.h"

// Fonction simple qui ne fait rien bouger
Action dummyFindAction(Animal *animal, Grid *grid, Position pos) {
    Action action = {{0, 0}, false};  // Ne bouge pas, ne mange pas
    return action;
}

// Pas de reproduction
Animal *noReproduce(Animal *animal) {
    return NULL;
}

int main() {
    printf("Creating simulation...\n");
    Simulation *sim = simulationCreate(5, 5);
    
    printf("Creating animal with energy=10...\n");
    Animal *animal = animalCreate(
        "TestAnimal",
        1,           // priority
        10,          // energy
        1,           // actionEnergy (perd 1 par step)
        5,           // eatEnergy
        dummyFindAction,
        noReproduce
    );
    
    Position pos = {2, 2};
    printf("Adding animal to simulation...\n");
    simulationAddAnimal(sim, animal, pos);
    
    printf("Running steps...\n");
    for (int i = 1; i <= 15; i++) {
        printf("Step %d - Energy: %d\n", i, animalGetEnergy(animal));
        simulationStep(sim);
        
        // Vérifier si l'animal est encore là
        Animal *check = gridGetAnimal(simulationGetGrid(sim), pos);
        if (!check) {
            printf("Animal died at step %d\n", i);
            break;
        }
    }
    
    printf("Freeing simulation...\n");
    simulationFree(sim);
    printf("SUCCESS - No crash!\n");
    
    return 0;
}