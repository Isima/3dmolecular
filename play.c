#include "play.h"

#include "strategy.h"
#include "espace3d.h"
#include "utility.h"
#include "output.h"

#include <stdio.h>

int iter = 0;

void play(game_t* gm, unsigned idk) {

	molecule_t* m = molecule(gm);
	tree_t* k = tree(treesFull(gm),idk);
	grid_t* g = grid(gm);

	//trichecycleStrat(m, k);

	getStrategiesTree2(m, k, sizeG(g));
	findCoords( k, g);
	getCost( m, k, g);

	switchCostMolecule( m, k);
	getUtilities( m, k);

	updateAllStratLRI( m, k, sizeG(g));
}

int checkUtilityMin(tree_t* k) {

	for (int i=0; i<sizeT(k); ++i)
		if (utilityV(k,i) < PMIN)
			return 0;

	printf("\nUtilités > %f\n", PMIN);
	return 1;
}

int check(game_t* gm, char* name) {

	molecule_t* m = molecule(gm);
	tree_t* k = tree(treesFull(gm),0);
	grid_t* g = grid(gm);
	backup_t* b = backup(gm);


	getStrategiesTree2(m, k, sizeG(g));

	//trichecycleStrat(m, k);

	findCoords(k, g);
	getCost( m, k, g);

	switchCostMolecule( m, k);
	getUtilities( m, k);

	if (newBest(b,k)) {
		for (int i=0; i<sizeT(k); ++i) {
			coordsA(m,idmV(k,i)) = coordsV(best(b),i);
		}
		//output(name, gm, ++iter);
		nbIters(b)++;
		//writeTree(best(b));
	}

	if (checkUtilityMin(k)) {
		for (int i=0; i<sizeT(k); ++i)
			coordsA(m,idmV(k,i)) = coordsV(k,i);
		return 1;
	}

	if (nbIters(b)>BLOCK) {
		printf("\n Pas de bonne solution trouvée\n");
		return 1;
	}

	//if (nbIters(b)%100 == 0)
		//printf("iter = %d\n", nbIters(b));
	
	return 0;
}

void initPlay(game_t* g, int nbStart) {

	int j;
	for (j=0; j<nbStart; ++j){
		getStrategiesTree2(molecule(g), tree(treesFull(g),0), nbStrat(g));
		findCoords(tree(treesFull(g),0), grid(g));
		getCost( molecule(g), tree(treesFull(g),0), grid(g));

		addCostMolecule( molecule(g), tree(treesFull(g),0));
	}
}

void playGame(game_t* g, char* name) {

	int k;
	initPlay(g,100);

	while(!check(g, name)) {
		for (k=0; k<CHECK; ++k) {
			play(g,0);
		}
	}
	
}
