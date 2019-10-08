#include "backup.h"

#include <stdlib.h>
#include <stdio.h> //test printf

backup_t* createBackup() {

	backup_t* out = malloc(sizeof(backup_t));

	best(out) = NULL;
	bestMedium(out) = 0;
	nbIters(out) = 0;

	return out;
}

tree_t* createCopyTree(tree_t* k) {

	tree_t* out = malloc(sizeof(tree_t));

	sizeT(out) = sizeT(k);
	verticesT(out) = malloc(sizeT(k)*sizeof(vertex_t));

	for (int i=0; i<sizeT(k); ++i) {
		nbEdgesV(out,i) = nbEdgesV(k,i);
		edgesV(out,i) = malloc(nbEdgesV(out,i)*sizeof(edge_t));
	}

	return out;
}

void copyTree(tree_t* k, tree_t* copy) {

	int i, j;

	for (i=0; i<sizeT(k); ++i) {
		idmV(copy,i) = idmV(k,i);
		coordsV(copy,i) = coordsV(k,i);
		utilityV(copy,i) = utilityV(k,i);
		directV(copy,i) = directV(k,i);
		localV(copy,i) = localV(k,i);
		indirectV(copy,i) = indirectV(k,i);
		

		for (j=0; j<nbEdgesV(k,i); ++j) {
			idE(copy,i,j) = idE(k,i,j);
			typeE(copy,i,j) = typeE(k,i,j);
			stratE(copy,i,j) = stratE(k,i,j);
		}
	}
}

double getMedium(tree_t* k) {

	double medium = 0, cpt = 0;

	for (int i=0; i<sizeT(k); ++i) {
		if (nbEdgesV(k,i) != 1) {
			medium += utilityV(k,i);
			cpt++;
		}
	}

	return medium/cpt;
}

int newBest(backup_t* b, tree_t* k) {

	double medium = getMedium(k);

	nbIters(b)++;

	if (medium > bestMedium(b)) {
		if(best(b) == NULL)
			best(b) = createCopyTree(k);

		copyTree(k,best(b));
		nbIters(b) = 0;
		bestMedium(b) = medium;

		printf("medium = %f\n", medium);

		return 1;
	}

	return 0;
}

int newBest2(backup_t* b, tree_t* k, double medium) {

	if (medium > bestMedium(b)) {
		if(best(b) == NULL)
			best(b) = createCopyTree(k);

		copyTree(k,best(b));
		nbIters(b) = 0;
		bestMedium(b) = medium;

		printf("medium = %f\n", medium);

		return 1;
	}

	return 0;
}

void deleteBackup(backup_t* b) {

	if (b!=NULL) {
		if (best(b) != NULL)
			deleteTree(best(b));
		free(b);
	}
}
