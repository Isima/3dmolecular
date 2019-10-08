#include "grid.h"

#include <stdlib.h> //malloc
#include <stdio.h>	//printf

/*
* Crée une grille.
*
* @return 	Retourne la grille créée.
*
*/
grid_t* createGrid() {
	grid_t* g = malloc(sizeof(grid_t));

	g->size = 0;
	strategies(g) = NULL;
	g->ideal = NULL;

	return g;
}

/*
* Supprime une grille.
*/
void deleteGrid(grid_t* g) {

	for (int i=0; i<nbGeometry; i++)
		free(g->ideal[i]);
	free(g->ideal);
	free(strategies(g));

	free(g);
}

/*
* Initialise les positions idéales d'un sommet de géométrie 4.
*
* @param 	p 	Adresse de stockage des points du tétraèdre.
*
*/
void initTetrahedron(point_t* p) {

	point_t normal = {0.0, 0.0, 1.0},
					tmp,
					zero = {0.0, 0.0, 0.0};

	double angle = 180 - (109.47/2);

	p[0] = initPoint( -LENGHT, 0.0, 0.0);
	p[1] = rotation( p[0], 109.47, normal);

	tmp = normalisation( addPoint( p[0], p[1]), -LENGHT);
	normal = plan( zero, normal, tmp);

	p[2] = rotation( tmp,  angle, normal);
	p[3] = rotation( tmp, -angle, normal);
}

/*
* Initialise les positions idéales d'un sommet de géométrie 3.
*
* @param 	p 	Adresse de stockage des points du triangle.
*
*/
void initTriangle(point_t* p) {

	point_t normal = {0,0,1};

	p[0] = initPoint( -LENGHT, 0,0);
	p[1] = rotation( p[0],  120.0, normal);
	p[2] = rotation( p[0], -120.0, normal);
}

/*
* Initialise les positions idéales d'un sommet de géométrie 2.
*
* @param 	p 	Adresse de stockage des points de la ligne.
*
*/
void initLine(point_t* p) {

	p[0] = initPoint(-LENGHT, 0.0, 0.0);
	p[1] = initPoint( LENGHT, 0.0, 0.0);
}

/*
* Initialise les positions idéales de toutes les géométries possibles.
*
* @return 	Retourne l'ensemble des positions initialisées pour toutes les géométries.
*
*/
point_t** initIdeal() {

	point_t** ideal = malloc(nbGeometry*sizeof(point_t*));

	for (int i=0; i<nbGeometry; i++)
		ideal[i] = malloc( (i+2)*sizeof(point_t));
	initLine( ideal[0]);
	initTriangle( ideal[1]);
	initTetrahedron( ideal[2]);

	return ideal;
}

/*
* Initialise les points d'un icosèdre comme stratégie de jeu.
*
* @return 	Retourne l'ensemble des points après initialisation.
*
*/
point_t* initIcosahedron() {
	int i, j;
	point_t* p = malloc( 12*sizeof(point_t));

	p[0] = initPoint(-1.0, 0.0, 0.0);
	p[1] = initPoint( 1.0, 0.0, 0.0);

	p[2] = rotation( p[0], 60, initPoint( 0.0, 0.0, 1.0));
	p[3] = rotation( rotation( p[0], 120, initPoint(0,0,1)), 36, p[0]);

	for (i=4, j=1; i<12; i+=2, j++) {
		p[i] = rotation( p[2], j*72, p[0]);
	}

	for (i=5, j=1; i<12; i+=2, j++) {
		p[i] = rotation( p[3], j*72, p[0]);
	}

	return p;
}

void addStrat(grid_t* g, point_t p) {

	strategies(g) = realloc(strategies(g), ++sizeG(g)*sizeof(point_t));

	strategy(g,sizeG(g)-1) = p;
}

void findNearest(grid_t* g, unsigned id, int size, int nb, int* indiceMin) {

	int i, j, k;
	double dist, distMin[6];

	for (i=0; i<nb; ++i)
		distMin[i] = -1;

	//pour tous les sommets
	for (i=0; i<size; ++i) {
		if (i != id) {
			/*Regarder s'il est parmis les plus proches.*/
			//Distance entre le point de départ et celui regardé à l'itération.
			dist = distance(strategy(g,i), strategy(g,id));

			//Comparer à ceux déjà trouvé
			for (j=0; j<nb; ++j) {
				if (distMin[j] == -1) {
					distMin[j] = dist;
					indiceMin[j] = i;
					j=nb;
				}
				else if (dist < distMin[j]) {
					for (k=nb-1; k>j; --k) {
						distMin[k] = distMin[k-1];
						indiceMin[k] = indiceMin[k-1];
					}
					distMin[j] = dist;
					indiceMin[j] = i;
					j=nb;
				}
			}
		}
	}
}

void rafineGrid(grid_t* g) {

	int indiceMin[6];

	int i, j, nb, size = sizeG(g);

	for (i=0; i<size; ++i) {
		if (i<12) nb = 5;
		else nb = 6;

		findNearest( g, i, size, nb, indiceMin);

		for (j=0; j<nb; ++j)
			if (i<indiceMin[j]) {
				//printf("Entre %d et %d on a %d\n", i, indiceMin[j],sizeG(g));
				addStrat(g, normalisation(middle(strategy(g,i), strategy(g,indiceMin[j])),1));
			}
	}

}

/*
* Initialise l'ensemble de la grille.
*
* @return 	Retourne l'ardresse de la grille initialisée.
*
*/
grid_t* initGrid() {
	grid_t* g = createGrid();

	g->ideal = initIdeal();

	g->size = 12;
	strategies(g) = initIcosahedron();

	rafineGrid(g);

	return g;
}

/*
* Chercher l'identifiant de la stratégie la plus proche du point A.
*
* @param 	g 	Grille de recherche.
* @param 	A 	Point de comparaison.
* @return 		Identifiant de la stratégie la plus proche de A.
*
*/
int findNearestStrat(grid_t* g, point_t A) {

	int imin = 0;

	for (int i=1; i<g->size; i++)
		if (distance( A, strategy(g,imin)) > distance( A, strategy(g,i)))
			imin = i;

	return imin;
}

/*
* Écrie une grille dans la sortie standard.
*
* @param 	g 	Adresse de la grille à écrire.
*
*/
void writeGrid(grid_t* g) {

	printf("grid \n");
}