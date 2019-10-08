#ifndef __UTILITY_H
#define __UTILITY_H

#include "game.h"

void getCost(molecule_t*, tree_t*, grid_t*);
void v2getCost(molecule_t*, tree_t*, grid_t*);
void addCostMolecule(molecule_t*, tree_t*);
void switchCostMolecule(molecule_t*, tree_t*);
void getUtilities(molecule_t*, tree_t*);

#endif
