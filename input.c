#include "input.h"

#include <stdio.h> //fopen, printf, fscanf, fclose
#include <stdlib.h> //exit

/**************************************/
/* INITIALISATION MOLÉCULE ************/
/**************************************/

/* Fonction qui va chercher une molécule dans un fichier .txt
*
* @param	inputname	Nom du fichier contenant la molécule.
* @return	m 				Molécule extraite à partir du fichier.
*
*/
molecule_t* readInput_txt(char* inputname, int nbStrat) {
	FILE* filestream = NULL;
	int i, size, type, id1, id2;
	molecule_t* m;
	
	filestream = fopen(inputname, "r");

	if (filestream == NULL) {
		printf("le fichier %s n'existe pas\n", inputname);
		exit(1);
	}
	
	fscanf(filestream, "%d", &size);

	m = createMolecule();
	
	for (i=0; i<size; ++i) {
		addAtom(m);
		fscanf(filestream, "%d %s", &type, symbolA(m,i));
	}

	while (fscanf(filestream, "%d %d %d", &id1, &id2, &type) > 0) {
		addBond(m, id1-1, id2-1, type, nbStrat);
	}
	
	fclose(filestream);

	return m;
}
