//Interface du module ball
#ifndef HEADERBALL_H
#define HEADERBALL_H

#include <string>
#include <vector>
#include "tools.h"

class Ball{
private:
		Cercle balle;
		double inclinaison;
		
public:
		Ball(double x, double y, double inclinaison, int nbCell); 
		Cercle get_cercle() const;
		double get_inclinaison() const;
		void set_x(double x);
		void set_y(double y);
};

void update_balls_db(double x, double y, double inclinaison,
					 int nbCell, int i, bool& a_ouvrir, bool& arret);
					 
void update_balls_db(double x, double y, double inclinaison, 
                     int nbCell);

void check_adherance(Cercle balle, int i, bool& a_ouvrir, bool& arret);

void check_balle_adherance(); 

void collis_ball_ball(Point centre, int nbCell, bool& a_ouvrir, 
					  bool& arret, size_t& i);
					  
void collis_ball_ball(int& nbCell);

void copie_ball_db(std::vector<Ball>& copie_balles);

void clear_static_balls();

void get_ball_db_size(size_t& nb_players);

void access_secret_ball_db(double& x, double& y, 
						   double& rayon, size_t& i);

void update_balls_position(int nbCell);

void erase_ball(size_t& i);

void effacer_balles(std::vector<int>& balles);

#endif
