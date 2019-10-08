#include "molecule.h"

#include <stdio.h> 	//printf
#include <stdlib.h> //malloc, realloc.
#include <string.h> //strcmp

#define deleteDirect(m,i) deleteCost(directA(m,i))
#define deleteIndirect(m,i) deleteCost(indirectA(m,i))
#define writeDirect(m,i) printf("direct : "); writeCost(directA(m,i))
#define writeIndirect(m,i) printf("indirect : "); writeCost(indirectA(m,i))

/*
* Crée une molécule.
*
* @return 	Adresse de la molécule créée.
*
*/
molecule_t* createMolecule() {

	molecule_t* m = malloc(sizeof(molecule_t));
	atomsM(m) = NULL;
	sizeM(m) = 0;

	return m;
}

/*
* Crée une liste de coûts.
*
* @return 	Retourne l'adresse de la liste.
*
*/
cost_t* createCost() {

	cost_t* c = malloc(sizeof(cost_t));

	c->begin = NULL;
	c->end = NULL;

	c->max = 1;
	c->min = 1;

	return c;
}

/*
* Ajoute un atome dans la molécule.
*
* @param 	m 	Molécule recevant un atome de plus.
*
*/
void addAtom(molecule_t* m) {

	int i = sizeM(m)++;

	atomsM(m) = realloc(atomsM(m), sizeM(m) * sizeof(atom_t));


	bondsA(m,i) = NULL;
	nbBondsA(m,i) = 0;
	symbolA(m,i)[0] = '\0';
	geometryA(m,i) = 0;

	coordsA(m,i) = initPoint(0,0,0);


	directA(m,i) = createCost();
	indirectA(m,i) = createCost();

}

/*
* Renvoie l'indice dans le voisinage de i de la liaison avec l'atome id.
*
* @param 	m 	Molécule.
* @param 	i 	Identifiant de l'atome principal.
* @param 	id 	Identifiant de l'atome à rechercher dans le voisinage.
* @return 		Renvoie l'indice de id dans le voisinage de i, -1 s'il n'y a pas de liaison (i, id).
*
*/
int getIndiceB(molecule_t* m, unsigned i, unsigned id) {

	for (int j=0; j<nbBondsA(m,i); ++j)
		if (idB(m,i,j) == id)
			return j;

	return -1;
}

/*
* Ajoute un voisin d'identifiant id dans le voisinage de i.
*
* @param 	m 				Molécule.
* @param 	i 				Identifiant de l'atome principal.
* @param 	id 				Identifiant du voisin de i.
* @param 	type 			Type de la liaison (simple, double, triple, hydrogène).
* @param 	nbStrat 	Nombre de stratégies.
*
*/
void addNeighbor(molecule_t* m, unsigned i, unsigned id, int type, int nbStrat) {

	if (getIndiceB(m,i,id) == -1) {

		int j = nbBondsA(m,i)++;

		bondsA(m,i) = realloc(bondsA(m,i), nbBondsA(m,i) * sizeof(bond_t));

		idB(m,i,j) = id;
		typeB(m,i,j) = type;

		bondsA(m,i)[j].probabilities = malloc(nbStrat*sizeof(double));

		for (int k=0; k<nbStrat; ++k)
			probaB(m,i,j,k) = 1.0/nbStrat;
	}
}

/* 
* Ajoute une liaison entre les atomes id1 et id2.
*
* @param 	m 			Molécule.
* @param 	id1 		Premier atome de la liaison.
* @param 	id2 		Second atome de la liaison.
* @param 	type 		Type de la liaison.
* @param 	nbStrat	Nombre de stratégies.
*
*/
void addBond(molecule_t* m, unsigned id1, unsigned id2, int type, int nbStrat) {

	int j;

	if (id1 < m->size && id2 < m->size) {
		if (type == 4) {
			for (j=0; j<nbBondsA(m,id1) && strcmp(symbolA(m,idB(m,id1,j)), "H"); ++j);
			
			id1 = idB(m,id1,j);
		}

		addNeighbor(m, id1, id2, type, nbStrat);
		addNeighbor(m, id2, id1, type, nbStrat);
	}
}

int nbBondsMolecule(molecule_t* m) {

	int i, cpt = 0;

	for (i=0; i<sizeM(m); i++)
		cpt += nbBondsA(m,i);

	return cpt/2;
}

/*
*	Trouve la valeur minimum de la liste.
*
* @param 	c 	Liste de coûts.
* @return 		Valeur minimum de la liste.
*
*/
double getMinCost(cost_t* c) {

	double min = -1;

	if (c->begin != NULL) {

		min = c->begin->val;

		for (list_t* curr = c->begin; curr!=NULL; curr = curr->next)
			if (min > curr->val)
				min = curr->val;
	}

	return min;
}

/*
*	Trouve la valeur maximum de la liste.
*
* @param 	c 	Liste de coûts.
* @return 		Valeur maximum de la liste.
*
*/
double getMaxCost(cost_t* c) {

	double max = -1;

	if (c->begin != NULL) {

		max = c->begin->val;

		for (list_t* curr = c->begin; curr!=NULL; curr = curr->next)
			if (max < curr->val)
				max = curr->val;
	}

	return max;
}

/*
* Ajoute un élément en fin de liste.
* 
* @param 	c 	Liste de coûts.
* @param 	val	Valeur à ajouter à la liste.
*
*/
void addCost(cost_t* c, double val) {

	if (c->begin == NULL) {

		c->begin = malloc(sizeof(list_t));
		c->end = c->begin;

		c->min = val;
		c->max = val;

	}
	else {

		c->end->next = malloc(sizeof(list_t));
		c->end = c->end->next;

		//Si la nouvelle valeur est meilleure que l'une des autres.
		if (c->min > val) c->min = val;
		if (c->max < val) c->max = val;
	}

	c->end->val = val;
	c->end->next = NULL;
}

/*
* Supprime le premier élément de la liste.
*
* @param 	c 	Liste de coûts.
*
*/
void removeCost(cost_t* c) {

	if (c->begin != NULL) {

		list_t* old = c->begin;

		c->begin = c->begin->next;

		//if (c->min == old->val) c->min = getMinCost(c);
		if (c->max == old->val) c->max = getMaxCost(c);

		free(old);
	}
}

/*
* Échange un nouvel élément avec le plus ancien.
*
* @param 	c 	Liste de coûts.
* @param 	val	Valeur du nouvel élément.
*/
void switchCost(cost_t* c, double val) {

	removeCost(c);
	addCost(c, val);
}

/*
* Supprime une liste complète de coût.
*
* @param 	c 	Liste des coûts.
*
*/
void deleteCost(cost_t* c) {

	list_t *curr = c->begin, *next;

	while (curr != NULL) {
		next = curr->next;
		free(curr);
		curr = next;
	}

	free(c);
}

/*
* Supprime une liaison.
*
* @param 	m 	Molécule.
* @param 	i 	Identifiant de l'atome.
* @param 	j 	Indice de la liaison dans le voisinage de i.
*
*/
void deleteBond(molecule_t* m, unsigned i, unsigned j) {

	free(bondsA(m,i)[j].probabilities);
}

/*
* Supprime un atome.
*
* @param 	m 	Molécule.
* @param 	i 	Identifiant de l'atome.
*
*/
void deleteAtom(molecule_t* m, unsigned i) {

	for (int j=0; j<nbBondsA(m,i); ++j)
		deleteBond(m,i,j);

	deleteDirect(m,i);
	deleteIndirect(m,i);

	free(bondsA(m,i));
}

/*
* Supprime une molécule.
*
* @param 	m 	Molécule.
*
*/
void deleteMolecule(molecule_t* m) {

	for (int i=0; i<sizeM(m); ++i)
		deleteAtom(m,i);

	free(atomsM(m));
	free(m);
}

/*
* Écrie une liste de coûts.
*
* @param 	c 	Liste des coûts.
*
*/
void writeCost(cost_t* c) {

	for (list_t* curr = c->begin; curr != NULL; curr = curr->next)
		printf(" %2.3f,", curr->val);

	printf("\n");
}


/*
* Écrie une liaison.
*
* @param 	m 			Molécule.
* @param 	i 			Identifiant de l'atome.
* @param 	j 			Indice de la liaison dans le voisinage de i.
* @param 	nbStrat	Nombre de stratégies.
*
*/
void writeAllBond(molecule_t* m, unsigned i, unsigned j, int nbStrat) {

	printf("%d (%d) : ", idB(m,i,j)+1, typeB(m,i,j));

	for (int k=0; k<nbStrat; ++k) {
		printf(" %2.3f,", probaB(m,i,j,k));
	}
	printf("\n");
}

/*
* Écrie un atome.
*
* @param 	m 			Molécule.
* @param 	i 			Identifiant de l'atome.
* @param 	nbStrat	Nombre de stratégies.
*
*/
void writeAllAtom(molecule_t* m, unsigned i, int nbStrat) {

	printf("%d %s (%d), coutD( %2.3f, %2.3f), coutI( %2.3f, %2.3f), coords( %2.3f, %2.3f, %2.3f), nbBonds %d :\n",
		i+1, symbolA(m,i), geometryA(m,i), DminA(m,i), DmaxA(m,i), IminA(m,i), ImaxA(m,i),
		coordsA(m,i).x, coordsA(m,i).y, coordsA(m,i).z, nbBondsA(m,i));

	writeDirect(m,i);
	writeIndirect(m,i);

	for (int j=0; j<nbBondsA(m,i); ++j)
		writeAllBond(m, i, j, nbStrat);

	printf("\n");
}

/*
* Écrie une molécule.
*
* @param 	m 			Molécule.
* @param 	nbStrat	Nombre de stratégies.	
*
*/
void writeAllMolecule(molecule_t* m, int nbStrat) {

	printf("size molecule = %d\n", sizeM(m));
	for (int i=0; i<sizeM(m); ++i)
		writeAllAtom(m, i, nbStrat);
	printf("\n");
}

/*
* Écrie un atome sans les coûts et les probabilités des stratégies.
*
* @param 	m 			Molécule.
* @param 	i 			Identifiant de l'atome.
*
*/
void writeAtom(molecule_t* m, unsigned i) {

	printf("%d %s (%d), nbBonds %d :",
		i+1, symbolA(m,i), geometryA(m,i), nbBondsA(m,i));

	for (int j=0; j<nbBondsA(m,i); ++j)
		printf(" %d (%d),", idB(m,i,j)+1, typeB(m,i,j));

	printf("\n");
}

/*
* Écrie une molécule sans les coûts et les probabilités des stratégies.
*
* @param 	m 			Molécule.
*
*/
void writeMolecule(molecule_t* m) {

	printf("size molecule = %d\n", sizeM(m));
	for (int i=0; i<sizeM(m); ++i)
		writeAtom(m, i);
	printf("\n");
}
