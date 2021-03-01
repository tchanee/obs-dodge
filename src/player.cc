#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cmath>
#include <utility>
#include "player.h"
#include "define.h"
#include "tools.h"
#include "error.h"

using namespace std;

Player::Player(double x, double y, int nbTouch, 
			   double counte, int nbCell)
   : joueur(x,y) 
	{
		nb_touch = nbTouch;
		count = counte;
		joueur.rayon = (COEF_RAYON_JOUEUR * (SIDE/nbCell));
	} 

Cercle Player::get_cercle() const 
{
	return joueur;
}

static vector<Player> player_secret_db;

void update_player_db(double x, double y, int nb_touch, double count,  
					  int nbCell, int i, bool& a_ouvrir, bool& arret)
{
	Cercle cerc( x, y);
	Player player(x, y, nb_touch, count, nbCell); 
	check_adherance_joueur(cerc, i, a_ouvrir, arret);
	player_secret_db.push_back(player);
}

void check_adherance_joueur(Cercle joueur, int i, 
							bool& a_ouvrir, bool& arret) 
{
	if( joueur.centre.x < -DIM_MAX ||
		joueur.centre.x >  DIM_MAX ||
		joueur.centre.y < -DIM_MAX ||
		joueur.centre.y >  DIM_MAX )
		{
			cout << PLAYER_OUT(i) << endl;
			arret = true;
			if(!a_ouvrir) exit(EXIT_FAILURE);
		}
}

void collis_joueur_joueur(vector<Player>& player_db, 
						  int nbCell, bool& a_ouvrir, bool& arret)
{
	if(!arret)
	{
		double marge_lecture = (COEF_MARGE_JEU/2) * (SIDE/nbCell);
		for(size_t i(0); i < player_db.size(); ++i) 
		{
			for(size_t j(i+1); j < player_db.size(); ++j) 
			{
				if(!arret)
				{
					if (distance(player_db[i].get_cercle().centre ,
					    player_db[j].get_cercle().centre) <
					    (2*(COEF_RAYON_JOUEUR*(SIDE/nbCell))
					    +marge_lecture)) 
					{
						cout << PLAYER_COLLISION(i+1, j+1) << endl;
						arret = true;
						if(!a_ouvrir) exit(EXIT_FAILURE);
					}
				}	
			}	
		}		
	}		
}

void copie_player_db(vector<Player>& copie_players)
{
	for(auto player : player_secret_db)
	{
		copie_players.push_back(player);
	}
}

int Player::get_nb_touch() const
{
	return nb_touch;
}
		
double Player::get_count() const
{
	return count;
}

void clear_static_players()
{
	player_secret_db = {};
}

void get_player_db_size(size_t& nb_players)
{
	nb_players = player_secret_db.size();
}

int get_player_db_size()
{
	return player_secret_db.size();
}

void access_secret_player_db(double& x, double& y, double& rayon, 
							 int& nb_touch, double& count, size_t& i)
{
	x = player_secret_db[i].get_cercle().centre.x;
	y = player_secret_db[i].get_cercle().centre.y;
	rayon = player_secret_db[i].get_cercle().rayon;
	nb_touch = player_secret_db[i].get_nb_touch();
	count = player_secret_db[i].get_count();
}

void access_secret_player_db(double& x, double& y, size_t i)
{
	x = player_secret_db[i].get_cercle().centre.x;
	y = player_secret_db[i].get_cercle().centre.y;
}

void access_secret_player_db(int& x, int& y, int& nbCell, size_t i)
{
	x = ((player_secret_db[i].get_cercle().centre.x + DIM_MAX) / 
		(SIDE/nbCell));
	y = ((DIM_MAX - player_secret_db[i].get_cercle().centre.y) /
		(SIDE/nbCell));
}

int access_secret_player_db_count(size_t& i)
{
	return player_secret_db[i].get_count();
}

void Player::set_x(double x)
{
	joueur.centre.x += x;
}

void Player::set_y(double y)
{
	joueur.centre.y += y;
}

void update_player_position(size_t i, int cible, int& nbCell)
{
	if(!(distance(player_secret_db[i].get_cercle().centre.x, 
				 player_secret_db[i].get_cercle().centre.y, 
				 player_secret_db[cible].get_cercle().centre.x,
				 player_secret_db[cible].get_cercle().centre.y)
				 <=(2*COEF_RAYON_JOUEUR + COEF_MARGE_JEU)*SIDE/nbCell))
	{
		if(player_secret_db[i].get_cercle().centre.x==player_secret_db[cible].get_cercle().centre.x)
		{
			if(player_secret_db[i].get_cercle().centre.y > player_secret_db[cible].get_cercle().centre.y)
			{
				player_secret_db[i].set_y(-(DELTA_T*COEF_VITESSE_JOUEUR*(SIDE/nbCell)));
				player_secret_db[cible].set_y((DELTA_T*COEF_VITESSE_JOUEUR*(SIDE/nbCell)));
			}
			else
			{
				player_secret_db[i].set_y((DELTA_T*COEF_VITESSE_JOUEUR*(SIDE/nbCell)));
				player_secret_db[cible].set_y(-(DELTA_T*COEF_VITESSE_JOUEUR*(SIDE/nbCell)));
			}
		}
		else 
		{
			double angle(atan((player_secret_db[cible].get_cercle().centre.y
						-player_secret_db[i].get_cercle().centre.y)/
						(player_secret_db[cible].get_cercle().centre.x
						-player_secret_db[i].get_cercle().centre.x)));
			if(((player_secret_db[cible].get_cercle().centre.x
			-player_secret_db[i].get_cercle().centre.x < 0)))
			{
				angle += M_PI;
			}
			player_secret_db[i].set_x((cos(angle)*
			(DELTA_T*COEF_VITESSE_JOUEUR*(SIDE/nbCell))));
			player_secret_db[i].set_y((sin(angle)*
			(DELTA_T*COEF_VITESSE_JOUEUR*(SIDE/nbCell))));
		}
	}
}

void update_indirect_position(int spot, size_t i, int& nbCell)
{
	if(player_secret_db[i].get_cercle().centre.x==((spot%nbCell)+0.5)*SIDE/nbCell-DIM_MAX)
	{
		if(player_secret_db[i].get_cercle().centre.y>(DIM_MAX-(spot/nbCell+0.5)*SIDE/nbCell))
		{
			player_secret_db[i].set_y(-(DELTA_T*COEF_VITESSE_JOUEUR*(SIDE/nbCell)));
		}
		else
		{
			player_secret_db[i].set_y((DELTA_T*COEF_VITESSE_JOUEUR*(SIDE/nbCell)));
		}
	}
	else
	{
		double angle(atan(((DIM_MAX-(spot/nbCell+0.5)*SIDE/nbCell)
					-player_secret_db[i].get_cercle().centre.y)/
					(((spot%nbCell)+0.5)*SIDE/nbCell-DIM_MAX
					-player_secret_db[i].get_cercle().centre.x)));
		if((((spot%nbCell)+0.5)*SIDE/nbCell-DIM_MAX
		-player_secret_db[i].get_cercle().centre.x) < 0)
		{
			angle += M_PI;
		}
		player_secret_db[i].set_x((cos(angle)*
		(DELTA_T*COEF_VITESSE_JOUEUR*(SIDE/nbCell))));
		player_secret_db[i].set_y((sin(angle)*
		(DELTA_T*COEF_VITESSE_JOUEUR*(SIDE/nbCell))));
	}
}

void Player::decrease_nb_touch()
{
	--nb_touch;
}

void Player::reset_count()
{
	count = 0;
}

void hurt_target(int cible)
{
	player_secret_db[cible].decrease_nb_touch();
	if(player_secret_db[cible].get_nb_touch() == 0)
	{
		if(!((size_t)cible == player_secret_db.size()-1))
		{
			swap(player_secret_db[cible], 
				 player_secret_db[player_secret_db.size()-1]);
		}
		player_secret_db.pop_back();
	}
}

void reset_count(size_t i)
{
	player_secret_db[i].reset_count();
}

void Player::increase_count()
{
	++count;
}

void increment_count()
{
	for(size_t i(0); i < player_secret_db.size(); ++i)
	{
		player_secret_db[i].increase_count();
	}
}

void fake_update(double& x, double& y, int& spot, size_t i, 
				 int& nbCell)
{
	if(x==((spot%nbCell)+0.5)*SIDE/nbCell-DIM_MAX)
	{
		if(y>(DIM_MAX-(spot/nbCell+0.5)*SIDE/nbCell))
		{
			y -= (DELTA_T*COEF_VITESSE_JOUEUR*(SIDE/nbCell));
		}
		else
		{
			y += DELTA_T*COEF_VITESSE_JOUEUR*(SIDE/nbCell);
		}
	}
	else
	{
		double angle(atan(((DIM_MAX-(spot/nbCell+0.5)*SIDE/nbCell)
					-y)/
					(((spot%nbCell)+0.5)*SIDE/nbCell-DIM_MAX
					-x)));
		if((((spot%nbCell)+0.5)*SIDE/nbCell-DIM_MAX
		-x) < 0)
		{
			angle += M_PI;
		}
		x += ((cos(angle)*
		(DELTA_T*COEF_VITESSE_JOUEUR*(SIDE/nbCell))));
		y += ((sin(angle)*
		(DELTA_T*COEF_VITESSE_JOUEUR*(SIDE/nbCell))));
	}
}
