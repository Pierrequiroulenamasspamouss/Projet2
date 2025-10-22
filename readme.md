il reste à corriger simulation.c :
    - operations de simulation pas dans le bon ordre je crois
    - nettoyer le code 
    - ajouter commentaires
    - completer le rapport
    - faire plein de tests pour voir si y'a pas des trucs bizarres dans la simulation ( ex: lapins fantômes, loups immortels,etc...)
    - trouver un equilibre entre loups et lapins -> j'arrive pas a plus de 250 steps la plupart du temps

fonction à lancer : 
./simulate -s 150 -t 1000 -w 0.001 -r 0.002 -g 0.1 -f test.gif


-s : size of the grid
-t : time (in max number of steps)
-w : wolves spawning probability
-r : rabbit spawning probability
-g : grass spawning probability
-f : name of the output file 