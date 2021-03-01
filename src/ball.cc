#include <iostream>
#include <vector>
#include <cmath>
#include "ball.h"


using namespace std;


Ball::Ball(double x, double y, double inclinaison, int nbCell)  
	: balle(x, y)
	{ 
		balle.rayon = (COEF_RAYON_BALLE * (SIDE/nbCell));
		this->inclinaison = inclinaison;
	}
	
Cercle Ball::get_cercle() const
{
	return balle;
}

static vector<Ball> ball_secret_db;

double Ball::get_inclinaison() const
{
	return inclinaison;
}

void Ball::set_x(double x)
{
	balle.centre.x = x;
}

void Ball::set_y(double y)
{
	balle.centre.y = y;
}

void check_adherance(Cercle balle, int i, bool& a_ouvrir, bool& arret) 
{
	if( balle.centre.x < -DIM_MAX ||
		balle.centre.x >  DIM_MAX ||
		balle.centre.y < -DIM_MAX ||
		balle.centre.y >  DIM_MAX )
		{
			cout << BALL_OUT(i) << endl;
			arret = true;
			if(!a_ouvrir) exit(EXIT_FAILURE);
		}
}

void check_balle_adherance() 
{
	for(size_t i(0); i < ball_secret_db.size(); ++i)
		if(ball_secret_db[i].get_cercle().centre.x < -DIM_MAX ||
		ball_secret_db[i].get_cercle().centre.x >  DIM_MAX ||
		ball_secret_db[i].get_cercle().centre.y < -DIM_MAX ||
		ball_secret_db[i].get_cercle().centre.y >  DIM_MAX )
		{
			erase_ball(i);
		}
}

void update_balls_db(double x, double y, double inclinaison, 
                     int nbCell, int i, bool& a_ouvrir, bool& arret)
{
	Ball balle(x, y, inclinaison, nbCell); 
	Cercle cerc( x, y);
	check_adherance(cerc, i, a_ouvrir, arret);
	ball_secret_db.push_back(balle);
}

void update_balls_db(double x, double y, double inclinaison, 
                     int nbCell)
{
	Ball balle(x, y, inclinaison, nbCell); 
	ball_secret_db.push_back(balle);
}

void collis_ball_ball(Point centre, int nbCell, bool& a_ouvrir, 
					  bool& arret, size_t& i)
{	
	if(!arret)
	{
		double marge_lecture = (COEF_MARGE_JEU/2) * (SIDE/nbCell);
		for(size_t j(0); j < i; ++j) 
		{
			if(!arret)
			{	
				if(distance(centre , 
				   ball_secret_db[j].get_cercle().centre)
				   <(2*(COEF_RAYON_BALLE*(SIDE/nbCell))+marge_lecture)) 			  
				{	
					cout << BALL_COLLISION(j+1, i+1) << endl;
					arret = true;
					if(!a_ouvrir) exit(EXIT_FAILURE);
				}
			}	
		}	
	}					
}

void collis_ball_ball(int& nbCell)
{	
	double marge_jeu = (COEF_MARGE_JEU) * (SIDE/nbCell);
	for(size_t i(0); i < ball_secret_db.size(); ++i) 
	{
		for(size_t j(i+1); j < ball_secret_db.size(); ++j)
		{
			if(distance(ball_secret_db[i].get_cercle().centre, 
			   ball_secret_db[j].get_cercle().centre)
			   <(2*(COEF_RAYON_BALLE*(SIDE/nbCell))+marge_jeu)) 			  
			{	
				if(ball_secret_db.size() != i+1)
				{
					ball_secret_db[i] = ball_secret_db.back();
					ball_secret_db.pop_back(); 
				}				
				else
				{
					ball_secret_db.pop_back();
				}
				if(ball_secret_db.size() != j+1)
				{
					ball_secret_db[j] = ball_secret_db.back();
					ball_secret_db.pop_back(); 
				}				
				else
				{
					ball_secret_db.pop_back();
				}
			}
		}		
	}				
}

void copie_ball_db(vector<Ball>& copie_balles)
{
	for(auto ball : ball_secret_db)
	{
		copie_balles.push_back(ball);
	}
}

void clear_static_balls()
{
	ball_secret_db = {};
}

void get_ball_db_size(size_t& nb_players)
{
	nb_players = ball_secret_db.size();
}

void access_secret_ball_db(double& x, double& y, 
						   double& rayon, size_t& i)
{
	x = ball_secret_db[i].get_cercle().centre.x;
	y = ball_secret_db[i].get_cercle().centre.y;
	rayon = ball_secret_db[i].get_cercle().rayon;
}

void update_balls_position(int nbCell)
{
	for(size_t i(0); i < ball_secret_db.size(); ++i)
	{
		ball_secret_db[i].set_x(ball_secret_db[i].get_cercle().centre.x+
		(cos(ball_secret_db[i].get_inclinaison())*
		(DELTA_T*COEF_VITESSE_BALLE*SIDE)/(nbCell)));
		ball_secret_db[i].set_y(ball_secret_db[i].get_cercle().centre.y+
		(sin(ball_secret_db[i].get_inclinaison())*
		(DELTA_T*COEF_VITESSE_BALLE*SIDE)/(nbCell)));
	}
}

void erase_ball(size_t& i)
{
	ball_secret_db[i] = ball_secret_db.back();
	ball_secret_db.pop_back(); 				
}

void effacer_balles(vector<int>& balles)
{
	int tmp(0);
	for(size_t i(0); i < balles.size(); ++i)
	{
		if(balles[i] == 1)
		{
			ball_secret_db[i] =ball_secret_db[balles.size()-1-tmp];
			++tmp;
		}
	}
	for(int i(0); i < tmp; ++i)
	{
		ball_secret_db.pop_back();
	}
}
