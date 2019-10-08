#include "strategy.h"

#include <stdlib.h>
#include <stdio.h>

#include <string.h> //triche

/*
* Choisie une stratégies en fonction d'un tableau de probabilité.
* Les stratégies impossibles ont une proba de 0.
*
* @param 	proba 	Tableau contenant les probabilités d'apparition des stratégies.
* @param 	nbStrat	Nombre de stratégies.
*
*/
int getStrategy(int* proba, int nbStrat) {

	int	deb = 0, end = 0, r = rand();

	for (int i=0; i<nbStrat; ++i)
		deb += proba[i];

	r = r%deb;

	for (int i=0; i<nbStrat; ++i) {
		if (proba[i] != 0) {
			deb = end;
			end += proba[i];
			if (deb <= r && r < end) {
				return i;
			}
		}
	}

	return -1;
}

int getStrategy2(double* proba, int nbStrat) {

	double curr, next = 0.0, r = (float)(rand()%100)/100;

	//Pour chaque proba
	for (int k=0; k<nbStrat; k++) {
		curr = next;
		next += proba[k];
		if (curr <= r && r < next)
			return k;
	}

	return -1;
}

/*
* Choisie une stratégie pour chaque arête de l'arbre.
* Deux arêtes d'un même sommet ne peuvent pas avoir la même stratégie.
*
* @param 	m 			Adresse de la molécule (tableau de proba de chaque arête).
* @param 	k 			Adresse de l'arbre de jeu choisi.
* @param 	nbStrat	Nombre de stratégies.
*
*/
void getStrategiesTree(molecule_t* m, tree_t* k, int nbStrat) {

	int j, idV, idE, idA, idB;
	int* tab = malloc(nbStrat*sizeof(int));

	for (idV=0; idV<sizeT(k); ++idV) { //Pour chaque sommet de l'arbre

		idA = idmV(k,idV); //Identifiant dans la molécule.
		if (nbBondsA(m,idA) == 1) {
			stratE(k,idV,0) = 0;
		}
		else {
			for (idE=0; idE<nbEdgesV(k,idV); ++idE) { //Pour chaque arête de idV
				if (typeE(k,idV,idE) != 0) { //Si l'arête existe dans l'arbre.
					idB = getIndiceB(m, idA, idmV(k,idE(k,idV,idE))); //Obtenir l'indice de la liaison correspondante dans la molécule.
					if (idE == 0) { //Si c'est la première du sommet mettre à jour tout le vecteur.
						
						for (j=0; j<nbStrat; ++j)
							tab[j] = probaB(m,idA,idB,j)*100; //les mettre sur 100			
					}
					else { //Sinon ne pas mettre à jour les stratégies déjà utilisées.
						for (j=0; j<nbStrat; ++j)
							if (tab[j] != 0)
								tab[j] = probaB(m,idA,idB,j)*100;
					}

					stratE(k,idV,idE) = getStrategy(tab, nbStrat); //Obtenir une stratégie.
					tab[stratE(k,idV,idE)] = 0; //Mettre la stratégie à 0 pour quelle ne puisse pas être choisie pas les autres.			
				}
			}
		}
	}
	free(tab);
}

void getStrategiesTree2(molecule_t* m, tree_t* k, int nbStrat) {

	int idV, idE, idA, idB;

	for (idV=0; idV<sizeT(k); ++idV) { //Pour chaque sommet de l'arbre

		idA = idmV(k,idV); //Identifiant dans la molécule.
		if (nbBondsA(m,idA) == 1) {
			stratE(k,idV,0) = 0;
		}
		else {
			for (idE=0; idE<nbEdgesV(k,idV); ++idE) { //Pour chaque arête de idV
				if (typeE(k,idV,idE) != 0) {
					idB = getIndiceB(m, idA, idmV(k,idE(k,idV,idE))); 
					stratE(k,idV,idE) = getStrategy2(bondsA(m,idA)[idB].probabilities, nbStrat);
				}
			}
		}
	}
}

/*
* Choisie la meilleure stratégie pour chaque arête de l'arbre.
*
* @param 	m 			Adresse de la molécule (tableau de proba de chaque arête).
* @param 	k 			Adresse de l'arbre de jeu choisi.
* @param 	nbStrat	Nombre de stratégies.
*
*/
void getBestStrategiesTree(molecule_t* m, tree_t* k, int nbStrat) {

	int best, j, idV, idA, idE, idB;

	for (idV=0; idV<sizeT(k); ++idV) { //Pour chaque sommet de l'arbre.
		idA = idmV(k,idV); //Identifiant dans la molécule.
		for (idE=0; idE<nbEdgesV(k,idV); ++idE) { //Pour chaque arête du sommet.

			if (typeE(k,idV,idE) != 0) { //Si l'arête fait partie de l'arbre.
				idB = getIndiceB(m, idA, idmV(k,idE(k,idV,idE))); //Obtenir l'indice de la liaison 
				best = 0;
				for (j=1; j<nbStrat; ++j)
					if (probaB(m,idA,idB,j) > probaB(m,idA,idB,best))
						best = idB;

				stratE(k,idV,idE) = best;
			}
		}
	}
}

/* Méthode LRI pour mettre à jour les probas
* Utilisation d'un facteur b entre 0 et 1
*
* @param 	m 			Adresse de la molécule (tableau de proba de chaque arête).
* @param 	k 			Adresse de l'arbre de jeu choisi.
* @param 	nbStrat	Nombre de stratégies.
*
*/
void updateAllStratLRI(molecule_t* m, tree_t* k, int nbStrat) {

	int idV, idA, idE, idB;

	for (idV=0; idV<sizeT(k); ++idV) { //Pour chaque sommet de l'arbre.
		idA = idmV(k,idV); //Identifiant dans la molécule.
		for (idE=0; idE<nbEdgesV(k,idV); ++idE) { //Pour chaque arête du sommet.

			if (typeE(k,idV,idE) != 0) { //Si l'arête fait partie de l'arbre.
				idB = getIndiceB(m, idA, idmV(k,idE(k,idV,idE))); //Obtenir l'indice de la liaison 
				
				//Mise à jour des vecteurs de probabilité
				for (int i=0; i<nbStrat; ++i) {
					if (i == stratE(k,idV,idE))
						probaB(m,idA,idB,i) += PARAMETREB * utilityV(k,idV) * (1-probaB(m,idA,idB,i));
					else
						probaB(m,idA,idB,i) -= PARAMETREB * utilityV(k,idV) * probaB(m,idA,idB,i);
				}
			}
		}
	}
}

void trichecycleStrat(molecule_t* m, tree_t* k) {

	for (int i=0; i<sizeT(k); ++i) {
		if (!strcmp(symbolA(m,idmV(k,i)), "C")) {
			stratE(k,0,0) = 0;
			stratE(k,0,1) = 3;
			stratE(k,0,2) = 35;

			stratE(k,6,0) = 0;
			//stratE(k,6,1) = 3;
			stratE(k,6,2) = 35;

			stratE(k,2,0) = 0;
			stratE(k,2,1) = 3;
			stratE(k,2,2) = 35;
			
			stratE(k,1,0) = 0;
			stratE(k,1,1) = 31;
			stratE(k,1,2) = 11;

			stratE(k,4,0) = 0;
			stratE(k,4,1) = 35;
			stratE(k,4,2) = 3;

			stratE(k,8,0) = 0;
			//stratE(k,8,1) = 31;
			stratE(k,8,2) = 3;

			
		}
		else
			stratE(k,i,0) = 0;
	}
}
