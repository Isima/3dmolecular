#include "initialisation.h"
#include "input.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h> //a retirer plus tard

void getGeometryAtom(molecule_t* m) {

	int i;

	for (i=0; i<sizeM(m); i++) {
		if (!strcmp(symbolA(m,i),"O")) geometryA(m,i) = 4;
		else geometryA(m,i) = nbBondsA(m,i);
	}
}

molecule_t* initMolecule(char* name, unsigned nbStrat) {

	molecule_t* m = readInput_txt(name, nbStrat);
	//Calcul des géométrie.
	getGeometryAtom(m);

	return m;
}

void initTree(molecule_t* m, tree_t* k, int* tab, unsigned root) {

	int j,
			idV,	//Identifiant du sommet dans l'arbre.
			idA,	//Identifiant de l'atome correspondant au sommet.
			idB;	//Identifiant du voisin de l'atome dans la molécule.
						//L'identifiant du voisin dans l'arbre peut être obtenu à partir de tab;

	tab[root] = addVertex(k, root, -1);

	for (idV=0; idV<sizeT(k); ++idV) {

		idA = idmV(k,idV);

		for (j=0; j<nbBondsA(m,idA); ++j) { //Pour chaque liaison de l'atome.

			idB = idB(m,idA,j);
			if (tab[idB] == -1) {//Si le voisin devient un fils.
				tab[idB] = addVertex(k, idB, idV);
				addEdge(k, idV, tab[idB], 1);
			}
			else if (tab[idB] > -1)
				addEdge(k, idV, tab[idB], 0);
		}
	}
}

trees_t* initFull(game_t* g) {

	int id, j;
	molecule_t* m = molecule(g);
	trees_t* l = createTrees();

	int* tab = malloc(sizeM(m)*sizeof(int));


	for (int i=0; i<sizeM(m); ++i) {
		//Mettre tous les sommets à jour pour les inclure dans le nouvel arbre.
		if (nbBondsA(m,i) != 1) {
			for (j=0; j<sizeM(m); ++j)
				tab[j] = -1;

			id = addTree(l);

			initTree(m,tree(l,id),tab, i);
		}
	}

	free(tab);

	return l;
}

trees_t* initCycle(game_t* g) {

	trees_t* l = createTrees();

	printf("initialisation : initCycle à faire\n");

	return l;
}

trees_t* initSimple(game_t* g) {

	int id, j;
	molecule_t* m = molecule(g);
	trees_t* l = createTrees();

	int* tab = malloc(sizeM(m)*sizeof(int));

	for (int i=0; i<sizeM(m); ++i) { //Pour chaque sommet
		if (nbBondsA(m,i) > 1) {
			//Mettre tous les sommets à -2;
			for (j=0; j<sizeM(m); ++j)
				tab[j] = -2;

			//Mettre i et ses voisins à -1
			tab[i] = -1;
			for (j=0; j<nbBondsA(m,i); ++j)
				tab[idB(m,i,j)] = -1;

			id = addTree(l);

			initTree(m,tree(l,id),tab, i);
		}
	}

	free(tab);

	return l;
}

game_t* initGame(char* name) {

	game_t* g = createGame();

	grid(g) = initGrid();
	molecule(g) = initMolecule(name, nbStrat(g));
	
	treesFull(g) = initFull(g);
	treesCycle(g) = initCycle(g);
	treesSimple(g) = initSimple(g);

	//writeMolecule(molecule(g));
	//writeTrees(treesSimple(g));

	return g;
}