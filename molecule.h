#ifndef __MOLECULE_H
#define __MOLECULE_H

#include "point.h"

/*
* m 	Adresse de la molécule.
*	i 	Identifiant de l'atome.
* j 	Identifiant du voisin dans le voisinage
* k 	Identifiant de la stratégie.
*/

//Molécule
#define sizeM(m) (m)->size
#define atomsM(m) (m)->elts

//Atom
#define bondsA(m,i) (m)->elts[(i)].elts
#define nbBondsA(m,i) (m)->elts[(i)].size
#define symbolA(m,i) (m)->elts[(i)].info.symbol
#define geometryA(m,i) (m)->elts[(i)].info.geometry
#define coordsA(m,i) (m)->elts[(i)].coords
#define directA(m,i) (m)->elts[(i)].direct
#define indirectA(m,i) (m)->elts[(i)].indirect

//Liaison
#define idB(m,i,j) (m)->elts[(i)].elts[(j)].id
#define typeB(m,i,j) (m)->elts[(i)].elts[(j)].type
#define probaB(m,i,j,k) (m)->elts[(i)].elts[(j)].probabilities[(k)]

//Direct
#define DmaxA(m,i) (m)->elts[(i)].direct->max
#define DminA(m,i) (m)->elts[(i)].direct->min
#define DbeginA(m,i) (m)->elts[(i)].direct->begin
#define DendA(m,i) (m)->elts[(i)].direct->end

//Indirect
#define ImaxA(m,i) (m)->elts[(i)].indirect->max
#define IminA(m,i) (m)->elts[(i)].indirect->min
#define IbeginA(m,i) (m)->elts[(i)].indirect->begin
#define IendA(m,i) (m)->elts[(i)].indirect->end

#define addDirect(m,i,v) addCost(directA(m,i), (v))
#define addIndirect(m,i,v) addCost(indirectA(m,i), (v))
#define switchDirect(m,i,v) switchCost(directA(m,i), (v))
#define switchIndirect(m,i,v) switchCost(indirectA(m,i), (v))

/**************************************/
/* MOLECULE ***************************/
/**************************************/

/**
* <code>list_t</code> est une structure représentant une liste chaînée.
*/
typedef struct list {

	double val;					/*!< Valeur de l'élément.*/
	struct list* next;	/*!< Pointeur sur le suivant.*/
	
} list_t;

/**
* <code>cost_t</code> est une structure qui sauvegarde les 100 derniers coûts d'un atome.
*/
typedef struct {

	list_t* begin;	/*!< Adresse du premier de la liste.*/
	list_t* end;		/*!< Adresse du dernier de la liste.*/
	double min;			/*!< Valeur minimum de la liste.*/
	double max;			/*!< Valeur maximum de la liste.*/
	
} cost_t;

/**
* <code>chemicalInfo_t</code> est une structure qui regroupe des données chimiques d'un atome.
*/
typedef struct {

	char symbol[2];			/*!< Type chimique de l'atome.*/
	unsigned geometry;	/*!< Géométrie de l'atome.*/
	
} chemicalInfo_t;

/**
* <code>bond_t</code> est une structure qui représente une liaison d'un atome
*/
typedef struct {

	int id; 						/*!< Identifiant du voisin dans la molécule.*/
	unsigned type; 					/*!< Type de liaison (simple, double, triple, hydrogène).*/
	double* probabilities;	/*!< Tableau de probabilité de tirage de chaque stratégie.*/
	
} bond_t;

/**
* <code>atom_t</code> est une structure qui représente un atome de la molécule.
*/
typedef struct {

	bond_t* elts;  				/*!< Liste des liaisons (voisins) de l'atome.*/
	unsigned size; 				/*!< Nombre de liaisons de l'atome.*/

	chemicalInfo_t info;	/*!< Informations chimiques de l'atome.*/

	point_t coords; 			/*!< Meilleures coordonnées.*/
	cost_t* direct;				/*!< Coûts directs de l'atome.*/
	cost_t* indirect;			/*!< Coûts indirects de l'atome.*/
	
} atom_t;

/**
* <code>molecule_t</code> est une structure qui représente la molécule.
*/
typedef struct {

	atom_t* elts;		/*!< Liste des atomes de la molécule.*/
	unsigned size;	/*!< Nombre d'atomes de la molécule.*/
	
} molecule_t;

molecule_t* createMolecule();
void addAtom(molecule_t*);
int getIndiceB(molecule_t*, unsigned, unsigned);
void addBond(molecule_t*, unsigned, unsigned, int, int);
int nbBondsMolecule(molecule_t*);
void addCost(cost_t*, double);
void switchCost(cost_t*, double);
void deleteMolecule(molecule_t*);
void writeAllMolecule(molecule_t*, int);
void writeMolecule(molecule_t*);

#endif
