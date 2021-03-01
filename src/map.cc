#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include "map.h"

using namespace std;

int Map::get_nbCell() const
{
	 return nbCell ;
}

void Map::set_nbCell(int n) 
{
	 nbCell = n ;
}

static vector<Carre> obstacle_secret_db;
static int static_nbCell(0);

void set_static_nbCell(int nbCell)
{
	static_nbCell = nbCell;
}

int get_static_nbCell()
{
	return static_nbCell;
}

void superposition_obstacle(int& lgn, int& col, bool& a_ouvrir, 
							bool& arret, size_t& i)
{
	for(size_t j(0); j < i; ++j)
	{
		if ((lgn==obstacle_secret_db[j].lgn)and
			(col==obstacle_secret_db[j].col) )
			{	
				cout << MULTI_OBSTACLE(lgn, col) << endl; 
				arret = true;
				if(!a_ouvrir) exit(EXIT_FAILURE);
			}
	}
}

void check_adherance(Carre obs, int nbCell, int i, 
					 bool& a_ouvrir, bool& arret)
{
	if(obs.lgn < 0 || obs.lgn >= nbCell )
	{
		cout << OBSTACLE_VALUE_INCORRECT(obs.lgn) << endl;
		arret = true;
		if(!a_ouvrir) exit(EXIT_FAILURE);
	}
	if(obs.col < 0 || obs.col >= nbCell)
	{
		cout << OBSTACLE_VALUE_INCORRECT(obs.col) << endl;
		arret = true;
		if(!a_ouvrir) exit(EXIT_FAILURE);
	}
}	

void update_obstacle_db(int lgn, int col, int  nbCell, int i, 
						bool& a_ouvrir, bool& arret)
{
	Carre obs(lgn, col);
	check_adherance(obs, nbCell, i, a_ouvrir, arret);
	obstacle_secret_db.push_back(obs);
}

void copie_obstacle_db(vector<Carre>& copie_obstacles)
{
	for(auto obstacle : obstacle_secret_db)
	{
		copie_obstacles.push_back(obstacle);
	}
}

void clear_static_nbCell()
{
	static_nbCell = 0;
}

void clear_static_obstacles()
{
	obstacle_secret_db = {};
}

void get_obstacles_db_size(size_t& nb_obstacles)
{
	nb_obstacles = obstacle_secret_db.size();
}

void access_secret_obstacles_db(int& lgn, int& col, size_t& i)
{
	lgn = obstacle_secret_db[i].lgn;
	col = obstacle_secret_db[i].col;
}

void erase_obstacle(size_t& i)
{
	obstacle_secret_db[i] = obstacle_secret_db.back();
	obstacle_secret_db.pop_back();
}

void effacer_obstacles(vector<int>& obs)
{
	int tmp(0);
	for(size_t i(0); i < obs.size(); ++i)
	{
		if(obs[i] == 1)
		{
			obstacle_secret_db[i] =obstacle_secret_db[obs.size()-1-tmp];
			++tmp;
		}
	}
	for(int i(0); i < tmp; ++i)
	{
		obstacle_secret_db.pop_back();
	}
}
