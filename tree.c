#include "tree.h"

#include <stdio.h> //printf
#include <stdlib.h> //malloc, realloc

/*
* Crée une liste d'arbre.
*
* @return 	Adresse de la liste d'arbre à créer.
*
*/
trees_t* createTrees() {

	trees_t* l = malloc(sizeof(trees_t)); //Alloue la mémoire de la liste.

	l->size = 0; //Met ne nombre d'arbre à 0.
	l->elts = NULL; //Initialise à NULL la liste d'arbres.

	return l;
}

/*
* Ajoute un arbre à une liste d'arbre.
*
* @param 	l 	Adresse de la liste dans laquelle ajouter l'arbre.
* @return  		Identifiant de l'arbre ajouté.
*
*/
unsigned addTree(trees_t* l) {

	tree_t* k = NULL;
	unsigned id = l->size++;

	l->elts = realloc(l->elts, l->size * sizeof(tree_t)); //Réalloue la mémoire en conséquence.

	k = tree(l,id);

	sizeT(k) = 0;
	verticesT(k) = NULL;

	return id;
}

int getIndiceEdge(tree_t* k, unsigned i, unsigned id) {

	for (int j=0; j<nbEdgesV(k,i); ++j)
		if (idE(k,i,j) == id)
			return j;

	return -1;
}

/*
* Ajoute une arête à un sommet si elle n'existe pas encore.
*
* @param 	k 		Adresse de l'arbre dans lequel l'arête est ajoutée.
* @param 	i 		Identifiant du sommet dans lequel est ajouté l'arête.
* @param 	id 		Identifiant du second sommet de l'arête.
* @param 	type 	Type de relation.
*
*/
void addEdge(tree_t* k, unsigned i, unsigned id, unsigned type) {

	if (getIndiceEdge(k,i,id) == -1) {

		edgesV(k,i) = realloc(edgesV(k,i), ++nbEdgesV(k,i)*sizeof(edge_t));

		idE(k,i,nbEdgesV(k,i)-1) = id;
		typeE(k,i,nbEdgesV(k,i)-1) = type;

		stratE(k,i,nbEdgesV(k,i)-1) = 0;
	}
}

/*
* Ajoute un sommet dans un arbre.
*
* @param 	k 			Adresse de l'arbre.
* @param 	idm 		Identifiant de l'atome correspondant dans ka molécule.
* @param 	father 	Identifiant dans l'arbre du père.
*
*/
unsigned addVertex(tree_t* k, unsigned idm, int father) {

	if (father < (int)sizeT(k)) {

		unsigned i = sizeT(k)++;

		verticesT(k) = realloc(verticesT(k), sizeT(k)*sizeof(vertex_t));

		idmV(k,i) = idm;
		edgesV(k,i) = NULL;
		nbEdgesV(k,i) = 0;
		coordsV(k,i) = initPoint(0,0,0);

		directV(k,i) = 0;
		indirectV(k,i) = 0;
		localV(k,i) = 0;
		utilityV(k,i) = 0;

		if (father != -1)
			addEdge(k,i,father,2);

		return i;
	}

	return -1;
}

/*
* Supprime un arbre de la liste.
*
* @param 	k  	Adresse de l'arbre à supprimer.
*
*/
void deleteTree(tree_t* k) {

	for (int i=0; i<sizeT(k); ++i) //Pour chaque sommet supprimer la liste des arêtes.
		free(edgesV(k,i));

	free(k->elts); //Supprime la liste de sommets.
	sizeT(k) = 0; //Remet le nombre de sommets à 0.
	k->elts = NULL; //Remet la liste de sommets à NULL.
}

/*
* Supprime la liste des arbres.
*
* @param 	l  	Adresse de la liste d'arbre à supprimer.
*
*/
void deleteTrees(trees_t* l) {

	for (int k=0; k<l->size; ++k) //Supprimer chaque arbre.
		deleteTree( tree(l,k));

	free(l->elts); //Supprimer la liste des arbres.
	free(l); //Supprimer la liste.
}

/*
* Écrie les données d'un sommet d'un arbre.
*
* @param 	k 	Adresse de l'arbre auquel appartient le sommet.
* @param 	i 	Identifiant dans l'arbre du sommet à écrire.
*
*/
void writeVertex(tree_t* k, unsigned i) {

	printf("%d (%d) : coords( %2.3f, %2.3f, %2.3f), utility( %2.3f, %2.3f, %2.3f, %2.3f) : size %d:\n",
	i, idmV(k,i)+1, coordsV(k,i).x, coordsV(k,i).y, coordsV(k,i).z, utilityV(k,i),
	directV(k,i), indirectV(k,i), localV(k,i), nbEdgesV(k,i));

	for (int j=0; j<nbEdgesV(k,i); ++j)
		printf(" %d(%d,%d),", idE(k,i,j), stratE(k,i,j), typeE(k,i,j));

	printf("\n");
}

/*
* Écrie les données d'un arbre.
*
* @param 	k 	Adresse de l'arbre à écrire.
*
*/
void writeTree(tree_t* k) {

	printf("size = %d :\n", sizeT(k));

	for (int i=0; i<sizeT(k); ++i)
		writeVertex( k, i);
}

/*
* Écrie l'ensemble des arbres sur la sortie standard
*
* @param 	l 	Adresse de la liste de arbres à écrire.
* 
*/
void writeTrees(trees_t* l) {

	for (int k=0; k<l->size; ++k) {
		printf("Tree n°%d, ", k);
		writeTree( tree(l,k));
		printf("\n");
	}
}
