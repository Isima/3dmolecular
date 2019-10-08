#include "espace3d.h"

#include <stdio.h> //test
#include <math.h>
#include "output.h"


//Pour tout les sommets trouvés les coordonnées
void findCoords(tree_t* k, grid_t* g) {

	int i, j;
	double alpha = 0;
	point_t normal = {0,0,0};

	//pour le 0.
	for (i=0; i<sizeT(k); ++i) {
		if (nbEdgesV(k,i) != 1) {
			j = 0;
			if (typeE(k,i,j) == 2) {
				getRotation( strategy(g,stratE(k,i,j)), subPoint( coordsV(k,idE(k,i,j)), coordsV(k,i)), &alpha, &normal);
				++j;
			}	
			while (j<nbEdgesV(k,i)) {
				if (typeE(k,i,j) == 1)
					coordsV(k,idE(k,i,j)) = addPoint( coordsV(k,i), normalisation(rotation(strategy(g,stratE(k,i,j)), alpha, normal), LENGHT));
				++j;
			}
		}
	}
}



void getDirect(molecule_t* m, tree_t* k, grid_t* g) {

	int i, j;
	double alpha, cost;
	point_t normal;
	point_t ideal[4];
	point_t curr[4];

	//Pour tous les sommets.
	for (i=0; i<sizeT(k); ++i) {
		if (nbEdgesV(k,i) == 1) directV(k,i) = 0;
		else {

			//Initialisation
			for (j=0; j<nbEdgesV(k,i); ++j) {
				ideal[j] = ideal(g, geometryA(m,idmV(k,i)))[j];
				curr[j] = subPoint(coordsV(k,idE(k,i,j)), coordsV(k,i));
				if (typeE(k,i,j) == 0)
					curr[j] = normalisation(curr[j], LENGHT);
			}
			
			//Première rotation sur le père
			getRotation( curr[0], ideal[0], &alpha, &normal);
			for (j=0; j<nbEdgesV(k,i); ++j)
				curr[j] = rotation( curr[j], alpha, normal);

			//Second rotation
			getRotation( initPoint( 0, curr[1].y, curr[1].z), initPoint( 0, ideal[1].y, ideal[1].z), &alpha, &normal);
			for (j=0; j<nbEdgesV(k,i); ++j)
				curr[j] = rotation( curr[j], alpha, normal);

			//Calcul des couts directs
			directV(k,i) = 0;
			for (j=1; j<nbEdgesV(k,i); ++j)
				directV(k,i) += distance(ideal[j], curr[j]);

			if (nbEdgesV(k,i) == 4) {
				cost = distance(ideal[1], curr[1]) + distance(ideal[2], curr[3]) + distance(ideal[3], curr[2]);
				if (cost < directV(k,i))
					directV(k,i) = cost;			
			}
		}
	}
}

void getLocal(tree_t* k) {

	int i, i2, idE;
	double dist;
	localV(k,0) = 0;
	for (i=1; i<sizeT(k); ++i) {
		localV(k,i) = 0;

		for (i2=0; i2 <sizeT(k); ++i2) {

			if (i != i2) {
				idE = getIndiceEdge( k, i, i2);

				if (idE != -1) { //i2 voisin de i
					if (typeE(k,i,idE) == 0)
						localV(k,idE(k,i,0)) = fabs( distance( coordsV(k,i2), coordsV(k,i)) - LENGHT);
				}
				else {
					dist = distance( coordsV(k,i), coordsV(k,i2));
					if (dist < ORBITE)
						localV(k,idE(k,i,0)) += ORBITE - dist;
				}
			}
		}
	}
}