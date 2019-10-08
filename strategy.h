#ifndef __STRATEGY_H
#define __STRATEGY_H

#include "game.h"

void getStrategiesTree(molecule_t*, tree_t*, int);
void getStrategiesTree2(molecule_t*, tree_t*, int);
void getBestStrategiesTree(molecule_t*, tree_t*, int);
void updateAllStratLRI(molecule_t*, tree_t*, int);

void trichecycleStrat(molecule_t*, tree_t*);

#endif
