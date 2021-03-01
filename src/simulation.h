//Interface du module simulation

#ifndef HEADERSIMULATION_H
#define HEADERSIMULATION_H

#include <string>
#include <vector>


void lecture_error_check(char *nom_fichier,bool& a_ouvrir, bool& arret);
						 
void lecture_error_check(std::string nom_fichier,
						 bool& a_ouvrir, bool& arret);

void get_nb_players(size_t& nb_players);

void get_player_specifications(double& x, double& y, double& rayon, 
							   int& nb_touch, double& count, size_t& i);

void get_nb_obstacles(size_t& nb_obstacles);

void get_obstacle_specifications(int& lgn, int& col, size_t& i);

void get_nb_balls(size_t& nb_balls);	
							   
void get_ball_specifications(double& x, double& y, 
							 double& rayon, size_t& i);
						
int copie_nbCell();

void clear_nbCell();

void clear_player_secret_db();

void clear_obstacle_secret_db();

void clear_ball_secret_db();

void save(std::string& save_file);

void simulate(bool&, bool&);

void reset_matrice();

void end_game();

#endif
