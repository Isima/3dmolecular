#include <stdlib.h> //srand()
#include <time.h>   //time()

#include <stdio.h> //Affichage test

#include "initialisation.h"
#include "play2.h"
#include "output.h"


int main(int argc, char** argv) {

	char* name = argv[1];
	int run = atoi(argv[2]);

	srand(time(NULL));

	game_t* g = initGame(name);

	//tree_t* k = tree(treesFull(g),0);

	//initPlay(g,100);

	//playGame(g, name);
	playGame2(g, name, run);
	output(name,g,run,bestMedium(backup(g)));

	//writeAllMolecule(molecule(g), 4);
	//writeMolecule(molecule(g));
	writeTree(best(backup(g)));

	
	deleteGame(g);

	return 0;
}