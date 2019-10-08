#ifndef __TREE_H
#define __TREE_H

#include "point.h"

/*
* l 	Liste d'arbre
* k 	Identifiant ou adresse de l'arbre.
* i 	Identifiant dans l'arbre.
* j 	Identifiant du voisin dans le voisinage.
*/

//Liste des arbres.
//#define nbTrees(l) (l)->size

//Arbre
#define tree(l,k) (l)->elts+(k) 
#define sizeT(k) (k)->size
#define verticesT(k) (k)->elts
#define aveUtility(k) (k)->aveUtility
#define minUtility(k) (k)->minUtility

//Sommet
#define idmV(k,i) (k)->elts[(i)].idm
#define edgesV(k,i) (k)->elts[(i)].elts
#define nbEdgesV(k,i) (k)->elts[(i)].size
#define coordsV(k,i) (k)->elts[(i)].coords
#define utilityV(k,i) (k)->elts[(i)].utility.total
#define directV(k,i) (k)->elts[(i)].utility.direct
#define indirectV(k,i) (k)->elts[(i)].utility.indirect
#define localV(k,i) (k)->elts[(i)].utility.local

//Arête
#define idE(k,i,j) (k)->elts[(i)].elts[(j)].id
#define typeE(k,i,j) (k)->elts[(i)].elts[(j)].type
#define stratE(k,i,j) (k)->elts[(i)].elts[(j)].strat

/**************************************/
/* TREE *******************************/
/**************************************/

/**
* <code>utility_t</code> est une structure qui regroupe les données relatives à l'utilité pour chaque sommet d'un arbre.
*/
typedef struct {

	double total;			/*!< Utilité du sommet sur le tour de jeu.*/
	double direct;		/*!< Somme des coûts directs d'un sommet.*/
	double indirect;	/*!< Somme des coûts indirects d'un sommet.*/
	double local;			/*!< Somme des coûts indirects locaux au sommet.*/
	
} utility_t;

/**
* <code>edge_t</code> est une structure qui représente une arête d'un sommet.
*/
typedef struct {
	
	unsigned id;		/*!< Identifiant du second sommet de l'arête dans l'arbre.*/
	unsigned type;	/*!< Type de liaison : 2 si père, 1 si fils, 0 si non lié.*/
	
	unsigned strat; /*!< Stratégie de l'arc pour un tour de jeu.*/
} edge_t;

/**
* <code>vertex_t</code> est une structure qui représente un sommet.
*/
typedef struct {
	
	unsigned idm;				/*!< Identifiant du sommet dans la molécule.*/
	//unsigned father;	/*!< Identifiant du père dans l'arbre.*/

	edge_t* elts;				/*!< Liste des arêtes du sommet pouvant apparaitre dans l'arbre.*/
	unsigned size;			/*!< Nombre d'arêtes dans l'arbre.*/

	point_t coords;			/*!< Coordonnées du sommet dans le tour de jeu.*/
	utility_t utility;	/*!< Utilité du sommet dans le tour de jeu.*/
} vertex_t;

/**
* <code>tree_t</code> est une structure qui représente un arbre de jeu.
*/
typedef struct {
	
	vertex_t* elts;	/*!< Liste des sommets de l'arbre.*/
	unsigned size;	/*!< Nombre de sommets de l'arbre.*/

	//double averageUtility;
	//double minimumUtility;
} tree_t;

/**
* <code>tree_t</code> est une structure qui regroupe les arbres de jeu.
*/
typedef struct {

	tree_t* elts;		/*!< Liste des arbres du jeu.*/
	unsigned size;	/*!< Nombre d'arbre de jeu.*/
	
} trees_t;

trees_t* createTrees();
unsigned addTree(trees_t*);
int getIndiceEdge(tree_t*, unsigned, unsigned);
void addEdge(tree_t*, unsigned, unsigned, unsigned);
unsigned addVertex(tree_t*, unsigned, int);
void deleteTree(tree_t*);
void deleteTrees(trees_t*);
void writeTree(tree_t*);
void writeTrees(trees_t*);

#endif
