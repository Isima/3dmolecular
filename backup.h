#ifndef __BACKUP_H
#define __BACKUP_H

#include "tree.h"
#include "molecule.h"

#define bestMedium(b) (b)->bestMedium
#define best(b) (b)->best
#define nbIters(b) (b)->nbIters

typedef struct {
	
	tree_t* best;
	double bestMedium;

	//list_t* blocks;
	//unsigned nbBlocks;
	unsigned nbIters;

} backup_t;

backup_t* createBackup();
int newBest(backup_t*, tree_t*);
int newBest2(backup_t*, tree_t*, double);
void deleteBackup(backup_t*);

#endif