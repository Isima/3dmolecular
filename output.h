#ifndef __OUTPUT_H
#define __OUTPUT_H

#include "game.h"

char* createDir(char *);
char* getBasename (char * );
void copytoDir(char* , char* , char* );
void outputGrid(game_t*);
void outputPoints(unsigned, point_t*, unsigned);
void output(char*, game_t*, int, double);

#endif
