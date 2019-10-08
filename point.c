#include "point.h"

#include <math.h> //M_PI
#include <stdio.h> //Affichage

/*
* Transforme des degrés en radians.
*
* @param 	a 	Degrés.
* @return 		Radians.
*
*/
double degreToRadian(double a) {
	return a * M_PI / 180;
}

/*
* Transforme des radians en degrés.
*
* @param 	a 	Radians.
* @return 		Degrés.
*
*/
double radianToDegre(double a) {
	return a * 180 / M_PI;
}

/*
* Initialise un point.
*
* @param 	x 	Premier paramètre du point.
* @param 	y 	Second paramètre du point.
* @param 	z 	Troisième paramètre du point.
* @return 		Point initialisé.
*
*/
point_t initPoint(double x, double y, double z) {

	point_t out = {x,y,z};

	return out;
}

/*
* Additionne deux points.
*
* @param 	A 	Premier point.
* @param 	B		Second point à additionner au premier.
* @return 		Point contenant l'addition des deux points d'entrée.
*
*/
point_t addPoint(point_t A, point_t B) {

	point_t out = { A.x+B.x, A.y+B.y, A.z+B.z};

	return out;
}

/*
* Soustrait deux points.
*
* @param 	A 	Premier point.
* @param 	B		Second point à soustraire au premier.
* @return 		Point contenant la soustraction A - B.
*
*/
point_t subPoint(point_t A, point_t B) {

	point_t out = { A.x-B.x, A.y-B.y, A.z-B.z};

	return out;
}
/*
* Multiplie un point par un scalaire.
*/
point_t mulPoint(point_t A, double scal) {

	point_t out = { scal*A.x, scal*A.y, scal*A.z};

	return out;
}

/*
* Renvoie d'opposé d'un point
*
* @param 	A 	Point de départ
* @return 		Opposé du point A.
*
*/
point_t oppPoint(point_t A) {
	
	point_t out = { -A.x, -A.y, -A.z};

	return out;
}

point_t middle(point_t A, point_t B) {

	return mulPoint( addPoint(A, B), 0.5);
}

/*
* Vérifie si deux points sont égaux.
*
* @param 	A 	Premier point.
* @param 	B 	Second point.
* @return 		1 si A=B, 0 sinon.
*
*/
int isEgalPoint(point_t A, point_t B) {

	if (A.x == B.x && A.y == B.y && A.z == B.z)
		return 1;

	return 0;
}

/*
* Vérifie si deux points sont opposés.
*
* @param 	A 	Premier point.
* @param 	B 	Second point.
* @return 		1 si A=-B, 0 sinon.
*
*/
int isOppPoint(point_t A, point_t B) {

	if (A.x == -B.x && A.y == -B.y && A.z == -B.z)
		return 1;

	return 0;
}

/*
* Calcule la distance entre deux points.
*
* @param 	A 	Premier point.
* @param 	B 	Second point.
* @return 		Distance entre A et B.
*
*/
double distance(point_t A, point_t B) {

	return sqrt(pow((A.x - B.x), 2) + pow((A.y - B.y), 2)	+ pow((A.z - B.z), 2));
}

/*
* Calcule l'angle entre trois points.
*
* @param 	A 	Premier point.
* @param 	B 	Second point.
* @param 	C 	Troisième point.
* @return 		Angle BAC ou CAB.
*
*/
double angle(point_t A, point_t B, point_t C) {

	double alpha, AB = distance(A,B), AC = distance(A,C), BC = distance(B,C);

	//Si A=B ou A=C
	if (!AB || !AC) {
		return 0;
	}

	alpha = radianToDegre( acos((pow(AC,2)+pow(AB,2)-pow(BC,2)) / (2*AC*AB)));

	//Si alpha = nan car AB et AC ont des directions opposées
	if (alpha != alpha)
		return 180;

	return alpha;
}

/*
* Modifie la distance par rapport au centre.
*
* @param 	vetor 	Vecteur à normaliser.
* @param 	length 	Distance au centre souhaitée.
* @return 				Nouveau point à distance length du centre dans la direction de vector.
*
*/
point_t normalisation(point_t vector, double length) {
	
	double z = sqrt(pow(length,2) / (pow(vector.x,2) + pow(vector.y,2) + pow(vector.z,2)));
	point_t out = {z * vector.x, z * vector.y, z * vector.z};

	//Si vector était (0,0,0), z = inf
	if (out.x != out.x)
		return initPoint(0,0,0);

	return out;
}

/*
* Calcule la normal du plan formé par les trois points.
* Si les 3 sont alignés, normal = {0,0,0}
*
* @param 	A 	Premier point.
* @param 	B 	Second point.
* @param 	C 	Troisième point.
* @return 		Normal du plan passant par A, B et C.
*
*/
point_t plan(point_t A, point_t B, point_t C) {

	point_t normal =
			{(B.y - A.y) * (C.z - A.z) - (B.z - A.z) * (C.y - A.y),
			 (B.z - A.z) * (C.x - A.x) - (B.x - A.x) * (C.z - A.z),
			 (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x)};

	return normalisation(normal,1);
}


/*
* Calcule la rotation d'un point en fonction de la normal d'un plan et d'un angle.
*/
point_t rotation(point_t A, double alpha, point_t vector) {

	if (alpha == 0 || isEgalPoint(vector, initPoint(0,0,0)))
		return A;

	point_t out;
	alpha = degreToRadian(alpha);
	vector = normalisation(vector,1);

	out.x = (vector.x*vector.x + (1 - vector.x*vector.x)*cos(alpha)) * A.x +
			(vector.x*vector.y * (1 - cos(alpha)) - vector.z * sin(alpha)) * A.y +
			(vector.x*vector.z * (1 - cos(alpha)) + vector.y * sin(alpha)) * A.z;
	out.y = (vector.y*vector.y + (1 - vector.y*vector.y)*cos(alpha)) * A.y +
			(vector.x*vector.y * (1 - cos(alpha)) + vector.z * sin(alpha)) * A.x +
			(vector.y*vector.z * (1 - cos(alpha)) - vector.x * sin(alpha)) * A.z;
	out.z = (vector.z*vector.z + (1 - vector.z*vector.z)*cos(alpha)) * A.z +
			(vector.x*vector.z * (1 - cos(alpha)) - vector.y * sin(alpha)) * A.x +
			(vector.y*vector.z * (1 - cos(alpha)) + vector.x * sin(alpha)) * A.y;

	return out;

}

/*
*	Trouver l'angle et la noramle d'une rotation centrée allant d'un point A à un point B.
*
* @param 	A 			Point de départ de la rotation à calculer.
* @param 	B 			Point d'arrivée de la rotation à calculer.
* @param 	alpha 	Angle calculé pour la rotation.
* @param 	normal 	Normale calculée du plan de la rotation.
*
*/
void getRotation(point_t A, point_t B, double* alpha, point_t* normal) {

	point_t center = {0,0,0};
	(*alpha) = angle(center, A,B);

	if ((*alpha) == 180) {
		(*normal) = plan(center, A, initPoint(B.x, B.y-1, B.z));
	}
	else {
		(*normal) = plan(center, A, B);

		//Pas forcément nécessaire car normal de centre, A, B = - normal de centre B, A
		if ((*alpha) != 0)
			if (distance(B, rotation(A, (*alpha), (*normal))) > 
					distance(B, rotation(A,-(*alpha), (*normal))) )
				(*alpha) = -(*alpha);
	}
}

/*
* Trouve l'angle d'une rotation centrée allant d'un point A à un point B dont la normale est déjà connu.
*
* @param 	A 			Point de départ de la rotation à calculer.
* @param 	B 			Point d'arrivée de la rotation à calculer.
* @param 	normal 	Normale du plan de la rotation.
* @return 				Angle de la rotation.
*
*/
double getAngleRotation(point_t A, point_t B, point_t normal) {

	point_t center = {0,0,0};
	double alpha = angle(center, A,B);

	if (alpha != 0)
		if (distance(B, rotation(A, alpha, normal)) > 
				distance(B, rotation(A,-alpha, normal)) )
			alpha = -alpha;

	return alpha;
}

/*
* Affiche un point
*
* @param 	A 	Point à afficher.
*
*/
void writePoint(point_t A) {

	printf("( %2.3f, %2.3f, %2.3f)", A.x, A.y, A.z);
}
