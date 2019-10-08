#include "play2.h"

#include "strategy.h"
#include "espace3d.h"
#include "utility.h"
#include "output.h"

#include <stdio.h>

double utilityMedium(tree_t* k) {
	double med = 0;

	for (int i=0; i<sizeT(k); ++i)
		med += utilityV(k,i);
	return (med/sizeT(k));
}

double utilityMin(tree_t* k) {
	double min = 1;

	for (int i=0; i<sizeT(k); ++i)
		if (min > utilityV(k,i))
			min = utilityV(k,i);
	return min;
}

double playPhase(game_t* g) {

	getStrategiesTree2(molecule(g), tree(treesFull(g),0), sizeG(grid(g)));
	findCoords(tree(treesFull(g),0), grid(g));
	getCost(molecule(g), tree(treesFull(g),0), grid(g));

	switchCostMolecule(molecule(g), tree(treesFull(g),0));
	getUtilities( molecule(g), tree(treesFull(g),0));

	updateAllStratLRI(molecule(g), tree(treesFull(g),0), sizeG(grid(g)));

	return utilityMedium(tree(treesFull(g),0));
}

double block(game_t* g) {

	int new_ = 0;
	double medBlock = 0, medPhase;

	for (int i=0; i<CHECK; ++i) {

		medPhase = playPhase(g);
		medBlock += medPhase;

		if (newBest2(backup(g), tree(treesFull(g),0), medPhase))
			new_ = 1;
	}

	if (new_) {
		for (int i=0; i<sizeT(tree(treesFull(g),0)); ++i) {
			coordsA(molecule(g),idmV(tree(treesFull(g),0),i)) = coordsV(best(backup(g)),i);
		}
	}

	nbIters(backup(g))++;

	return medBlock/CHECK;
}

void initPlay2(game_t* g, int nbStart) {

	int j;
	for (j=0; j<nbStart; ++j){
		getStrategiesTree2(molecule(g), tree(treesFull(g),0), nbStrat(g));
		findCoords(tree(treesFull(g),0), grid(g));
		getCost( molecule(g), tree(treesFull(g),0), grid(g));

		addCostMolecule( molecule(g), tree(treesFull(g),0));
	}
}

void playGame2(game_t* g, char* InputFile, int run) {

	int k=0;
	double medium;
	initPlay2(g,100);
	FILE* filestream = NULL;

	char outputname[512];
  char* name = getBasename (InputFile);
  char* dirName = createDir(name);

  sprintf(outputname, "%s/%s_run%d.txt", dirName, name, run);

  filestream = fopen(outputname, "w");

  fprintf(filestream, "n° 	mediumBlock 	mediumBest\n");

	while(nbIters(backup(g))<BLOCK) {
		medium = block(g);
		fprintf(filestream, "%d 	%f 	%f\n", ++k, medium, bestMedium(backup(g)));
	}
	
	fclose(filestream);
}

