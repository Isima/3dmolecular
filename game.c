#include <stdlib.h>

#include "game.h"

/*
* Crée une jeu.
* 
* @return 	Retourne le jeu créé.
*
*/
game_t* createGame() {

	game_t* g = malloc(sizeof(game_t));

	molecule(g) = NULL;
	grid(g) = NULL;
	treesFull(g) = NULL;
	treesCycle(g) = NULL;
	treesSimple(g) = NULL;

	backup(g) = createBackup();

	return g;
}

/*
* Supprime un jeu.
* 
* @param 	g 	Adresse du jeu à supprimer.
*
*/
void deleteGame(game_t* g) {

	deleteMolecule(molecule(g));
	deleteGrid(grid(g));
	deleteTrees(treesFull(g));
	deleteTrees(treesCycle(g));
	deleteTrees(treesSimple(g));

	deleteBackup(backup(g));

	free(g);
}
