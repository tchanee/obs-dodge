//interface tools
#include <string>
#include <vector>
#include "define.h"
#include "error.h"

#ifndef HEADERTOOLS_H
#define HEADERTOOLS_H

class Point
{
public:	
	double x;
	double y;
	Point();
	Point(double, double);
};

class Vecteur
{
public:
	double coeff_x;
	double coeff_y;
};

class Segment
{
public:	
	Point A;
	Point B;
};

class Cercle
{
public:	
	Point centre;
	double rayon;
	Cercle(double, double);
};

class Carre
{ 
public:	
	  int lgn;
	  int col;
	  Carre(int lgn, int col); 
};

double distance(Point objet1, Point objet2);
double distance(Point objet1, double x, double y);
double distance(double x1, double y1, double x2, double y2);

#endif
