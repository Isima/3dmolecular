#include "output.h"

#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
*/
char* createDir(char *input) {
  char* dirName = malloc (256 * sizeof(char));
  
  mkdir("Results", 0755);
  sprintf(dirName,"Results/%s_%d_%d", input, (int)(PDIRECT*100), (int)(PINDIRECT*100));
  mkdir(dirName,0755);

  return dirName;
}

/**
*/
char* getBasename (char * in) {
  char* r = malloc (256 * sizeof (char) );  
  char *start;
  int i;

  start = strrchr(in, '/');

  for (i=0; start[i+1] != '.'; ++i) {
    r[i] = start[i+1];
  }
  r[i] = '\0';
  return r;  
}

/**
*/
void copytoDir(char* InputFile, char* dirName, char* name) {
  char cmd[512];

  sprintf(cmd, "cp %s %s/%s.xyz", InputFile, dirName, name);
  if (system(cmd) == -1) {
    printf("%s",cmd);
    exit(0);
  }
}

void writeMolXYZ(char* output, molecule_t* m) {
	FILE* filestream = NULL;

	filestream = fopen(output, "w");
	fprintf(filestream, "%d\n\n", sizeM(m));

	for (int i=0; i<sizeM(m); i++) {
		fprintf(filestream, "%s %f %f %f\n", symbolA(m,i),
			coordsA(m,i).x, coordsA(m,i).y, coordsA(m,i).z);
	}

  fclose(filestream);
}

void writeMoleculeMol2(char* output, molecule_t* m)  {
  FILE* filestream = NULL;
  int ret, i, j, l;

  filestream = fopen(output, "w");
  ret = fprintf(filestream, "@<TRIPOS>MOLECULE\n*****\n");
  ret = fprintf(filestream, " %d %d 0 0 0\n", sizeM(m), nbBondsMolecule(m));
  ret = fprintf(filestream, "SMALL\nGASTEIGER\n\n");

  //Ecriture des sommets
  ret = fprintf(filestream, "@<TRIPOS>ATOM\n");
  for (i=0; i<sizeM(m); i++) {

    ret = fprintf(filestream, " %3d %s", i+1, symbolA(m,i));


    if (coordsA(m,i).x < 0)
      ret = fprintf(filestream, "   %3.4f", coordsA(m,i).x);
    else
      ret = fprintf(filestream, "    %3.4f", coordsA(m,i).x);
    if (coordsA(m,i).y < 0)
      ret = fprintf(filestream, "   %3.4f", coordsA(m,i).y);
    else
      ret = fprintf(filestream, "    %3.4f", coordsA(m,i).y);
    if (coordsA(m,i).z < 0)
      ret = fprintf(filestream, "   %3.4f", coordsA(m,i).z);
    else
      ret = fprintf(filestream, "    %3.4f", coordsA(m,i).z);

    ret = fprintf(filestream, " %s\n", symbolA(m,i));
  }

  //Ecriture des liens
  ret = fprintf(filestream, "\n@<TRIPOS>BOND\n");
  for (i=0, l=1; i<sizeM(m); i++)
    for (j=0; j<nbBondsA(m,i); j++)
      if (i < idB(m,i,j)) {
        ret = fprintf(filestream, " %3d %3d %3d %3d\n", l, i+1, idB(m,i,j)+1, typeB(m,i,j));
      }

  fclose(filestream);

  if (ret<0) {
    printf("L'écriture du fichier %s ne s'est pas bien passé.\n", output);
    exit(2);
  }

}

void writeGridXYZ(char* output, grid_t* g) {
	FILE* filestream = NULL;

	filestream = fopen(output, "w");
	fprintf(filestream, "%d\n\n", sizeG(g));

	for (int i=0; i<sizeG(g); i++) {
		fprintf(filestream, "C %f %f %f\n",
			strategy(g,i).x, strategy(g,i).y, strategy(g,i).z);
	}

  fclose(filestream);
}

void outputGrid(game_t* g) {

	char outputname[512];

  sprintf(outputname, "Results/grid%d.xyz", nbStrat(g));
  writeGridXYZ(outputname, grid(g));
}

void outputPoints(unsigned i, point_t* p, unsigned size) {

  FILE* filestream = NULL;
  char outputname[512];
  sprintf(outputname, "Results/cycle1/v%d.xyz", i);

  filestream = fopen(outputname, "w");
  fprintf(filestream, "%d\n\n", size);

  for (int i=0; i<size; i++) {
    fprintf(filestream, "C %f %f %f\n",
      p[i].x, p[i].y, p[i].z);
  }
  
  fclose(filestream);

}

void output(char* InputFile, game_t* g, int i, double med) {

	char outputname[512];
  char* name = getBasename (InputFile);
  char* dirName = createDir(name);

  sprintf(outputname, "%s/%s_run%d_%3.3f.mol2", dirName, name, i, med);
  writeMoleculeMol2(outputname, molecule(g));

  //if (i==0) {
    sprintf(outputname, "%s/%s_run%d.xyz", dirName, name, i);
    writeMolXYZ(outputname, molecule(g));
  //}

}