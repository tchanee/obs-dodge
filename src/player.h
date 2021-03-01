//Interface du module player
#include <string>
#include <vector>
#include "tools.h"

#ifndef HEADERPLAYER_H
#define HEADERPLAYER_H

class Player
{
private:	
	Cercle joueur;
	int nb_touch; 
	double count;
		
public:	
	Player(double x, double y, int nbTouch, double counte, int nbCell);
	Cercle get_cercle() const; 
	int get_nb_touch() const;
	double get_count() const;
	void set_x(double x);
	void set_y(double y);
	void decrease_nb_touch();
	void increase_count();
	void reset_count();
};
 
void update_player_db(double x, double y, int nb_touch, double count, 
					  int nbCell, int i, bool& a_ouvrir, bool& arret);
 
void check_adherance_joueur(Cercle joueur, int i, 
							bool& a_ouvrir, bool& arret);
 
void collis_joueur_joueur(std::vector<Player>& player_db, 
						  int nbCell, bool& a_ouvrir, bool& arret);
					
void copie_player_db(std::vector<Player>& copie_players);	

void clear_static_players();

void get_player_db_size(size_t& nb_players);

int get_player_db_size();	

void access_secret_player_db(double& x, double& y, double& rayon, 
							 int& nb_touch, double& count, size_t& i);	
							 
void access_secret_player_db(double& x, double& y, size_t i);

void access_secret_player_db(int& x, int& y, int& nbCell, size_t i);

int access_secret_player_db_count(size_t& i);

void update_player_position(size_t i, int cible, int& nbCell);

void update_indirect_position(int spot, size_t i, int& nbCell);

void hurt_target(int cible);

void reset_count(size_t i);	

void increment_count();	

void fake_update(double& x, double& y, int& targeted_spot, size_t i, 
				 int& nbCell);

#endif

