#include "utility.h"
#include "espace3d.h"

#include <stdlib.h>
#include <stdio.h>
#include "output.h"

double getIndirect(tree_t* k, unsigned i, unsigned h) {

	double cost = localV(k,i)/h;

	for (int j=0; j<nbEdgesV(k,i); ++j) {
		if (typeE(k,i,j) == 1)
			cost += getIndirect( k, idE(k,i,j), h+1);
	}

	return cost;
}

void getCost(molecule_t* m, tree_t* k, grid_t* g) {

	getDirect(m,k,g);
	getLocal(k);

	for ( int i=0; i<sizeT(k); ++i) {
		if (nbEdgesV(k,i) != 1)
			indirectV(k,i) = getIndirect( k, i, 1);
	}
}

void addCostMolecule(molecule_t* m, tree_t* k) {

	for (int i=0; i<sizeT(k); ++i)
		if (nbEdgesV(k,i) == nbBondsA(m,idmV(k,i))) {
			addDirect( m, idmV(k,i), directV(k,i));
			addIndirect( m, idmV(k,i), indirectV(k,i));
		}
}

void switchCostMolecule(molecule_t* m, tree_t* k) {

	for (int i=0; i<sizeT(k); ++i)
		if (nbEdgesV(k,i) == nbBondsA(m,idmV(k,i))) {
			switchDirect( m, idmV(k,i), directV(k,i));
			switchIndirect( m, idmV(k,i), indirectV(k,i));
		}
}

void getUtilities(molecule_t* m, tree_t* k) {

	for (int i=0; i<sizeT(k); ++i) {
		if (nbEdgesV(k,i) == 1)
			utilityV(k,i) = 1;
		else {
			if (DmaxA(m,idmV(k,i)) != DminA(m,idmV(k,i))) {
				utilityV(k,i) = PDIRECT * ( DmaxA(m,idmV(k,i)) - directV(k,i) ) /
														( DmaxA(m,idmV(k,i)) - DminA(m,idmV(k,i)) );
			}
			else 
				utilityV(k,i) = PDIRECT;

			if (ImaxA(m,idmV(k,i)) != IminA(m,idmV(k,i))) {
				utilityV(k,i) += PINDIRECT * ( ImaxA(m,idmV(k,i)) - indirectV(k,i) ) /
														( ImaxA(m,idmV(k,i)) - IminA(m,idmV(k,i)) );
			}
			else
				utilityV(k,i) += PINDIRECT;
		}
	}
}