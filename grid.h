#ifndef __GRID_H
#define __GRID_H

#include "point.h"

#define sizeG(g) (g)->size
#define strategies(g) (g)->elts
#define strategy(g,i) (g)->elts[(i)]
#define ideal(g,i) (g)->ideal[(i)-2] //i la géométrie

#define nbGeometry 3

/**************************************/
/* GRID *******************************/
/**************************************/

/**
* <code>grid_t</code> est une structure qui contient les stratrégies des joueurs et la géométrie idéale des atomes.
*/
typedef struct {
	
	point_t* elts;			/*!< Ensemble des stratégies possibles (directions).*/
	unsigned size;			/*!< Nombre de stratégies.*/

	point_t** ideal;	/*!< Positions idéales des voisins d'un atome en fonction de sa géométrie.*/

} grid_t;

grid_t* createGrid();
void deleteGrid(grid_t*);
grid_t* initGrid();

#endif
