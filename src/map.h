//Interface du module map

#include <string>
#include <vector>
#include "tools.h"

#ifndef HEADERMAP_H
#define HEADERMAP_H

class Map
{
private:
	int nbCell;
	
public:
	int get_nbCell() const;
	void set_nbCell(int n);
};

void set_static_nbCell(int nbCell);

int get_static_nbCell();

void superposition_obstacle(int& lgn, int&col, bool& a_ouvrir, 
							bool& arret, size_t& i);

void check_adherance(Carre obs, int nbCell, int i, 
					 bool& a_ouvrir, bool& arret);

void update_obstacle_db(int lgn, int col, int  nbCell, int i, 
						bool& a_ouvrir, bool& arret);

void copie_obstacle_db(std::vector<Carre>& copie_obstacles);

void clear_static_nbCell();

void clear_static_obstacles();

void get_obstacles_db_size(size_t& nb_obstacles);

void access_secret_obstacles_db(int& lgn, int& col, size_t& i);

void erase_obstacle(size_t& i);

void effacer_obstacles(std::vector<int>& obs);

#endif
