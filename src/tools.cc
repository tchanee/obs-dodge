#include <iostream>
#include <cmath>
#include "tools.h"
using namespace std;

Point::Point()
{
	x = 0;
	y = 0;
}

Point::Point(double x, double y)
:x(x), y(y)
{
}

Cercle::Cercle(double x, double y)
{
	centre.x = x;
	centre.y = y;
}		

Carre::Carre(int lgn, int col)
	:lgn(lgn), col(col) 
{	
}

double distance(Point objet1, Point objet2)
{
        double dist;
        dist = sqrt( (pow((objet1.x - objet2.x),2) +
					  pow((objet1.y - objet2.y),2) ));
        return dist;
}

double distance(Point objet1, double x, double y)
{
        double dist;
        dist = sqrt( (pow((objet1.x - x),2) + pow((objet1.y - y),2) ));
        return dist;
}  

double distance(double x1, double y1, double x2, double y2)
{
	x1 = sqrt( (pow((x1 - x2),2) +
				pow((y1 - y2),2) ));
	return x1;
}
