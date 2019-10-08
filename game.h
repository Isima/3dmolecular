#ifndef __GAME_H
#define __GAME_H

#include "molecule.h"
#include "tree.h"
#include "grid.h"
#include "backup.h"

#define molecule(g) g->molecule
#define grid(g) g->grid
#define treesFull(g) g->full
#define treesCycle(g) g->cycle
#define treesSimple(g) g->simple
#define nbFull(g) (g)->full->size
#define backup(g) (g)->best

#define nbAtoms(g) g->molecule->size
#define nbStrat(g) g->grid->size

/**************************************/
/* GAME *******************************/
/**************************************/

typedef struct {
	
	molecule_t* molecule;
	grid_t* grid;

	trees_t* full;
	trees_t* cycle;
	trees_t* simple;

	backup_t* best; 

} game_t;

game_t* createGame();
void deleteGame(game_t*);

#endif
