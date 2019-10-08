#ifndef __ESPACE3D_H
#define __ESPACE3D_H

#include "game.h"

void findCoords(tree_t*, grid_t*);
void initMinIdeal(point_t*, point_t*, unsigned);
void getDirect(molecule_t*, tree_t*, grid_t*);
void getLocal(tree_t*);

#endif
