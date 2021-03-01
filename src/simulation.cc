#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <cmath>
#include <utility> 
#include "simulation.h"
#include "player.h"
#include "map.h"
#include "error.h"
#include "ball.h"
#include "tools.h"

typedef std::vector<std::vector<double>> matrice;

using namespace std;

enum Etat_lecture{NB_CELLULES, NB_PLAYERS, PLAYERS, NB_OBSTACLES,
		          OBSTACLES  , NB_BALLS  , BALLS  , FIN};
		          
static int etat(NB_CELLULES);
static int i(0); 
static int total(0); 
static int nbCell(0);
static bool initialisee(false);

void delegation_de_lecture(std::string ligne, 
						   bool& a_ouvrir, bool& arret);

void copie_elements_jeu(vector<Player>& copie_players,
						vector<Carre>&  copie_obstacles,
						vector<Ball>&	copie_balls);

void collis_joueur_ball(Point centre_balle, vector<Player>& player_db,
						int nbCell, bool& a_ouvrir, bool& arret, 
						size_t& j);
						
void collisions(bool& a_ouvrir, bool& arret);

void collis_ball_obs(vector<Carre>& obstacle_db, Point centre,
					int nbCell, bool& a_ouvrir, bool& arret, size_t& j);
					
void collis_ball_obs(vector<Carre>& obs, vector<Ball>& balles, 
					 int& nbCell);
							
void collis_play_obs(int& lgn, int& col, vector<Player>& player_db, 
					int nbCell, bool& a_ouvrir, bool& arret, size_t& i);
					
bool collis_play_obs(int nbCell, double& x, double& y);																				

void check_adherance_horizontalle_verticalle_player 
	(int& lgn, int&col , vector<Player>& player_db, 
     int nbCell, bool& a_ouvrir, bool& arret, size_t& i);
     
void check_adherance_horizontalle_verticalle_player
    (vector<Carre>& obs, double& x, double& y, int nbCell, 
    bool& colli);    
     
void changement_base_player_obs
    (int& lgn, int& col, vector<Player>& player_db, 
     int nbCell, bool& a_ouvrir, bool& arret, size_t& i, size_t& j); 
     
void changement_base_player_obs(vector<Carre>& obs, int nbCell, 
								double& x, double& y, 
								bool& colli, size_t& j);
      
void check_adherance_horizontalle_verticalle_balle
    (vector<Carre>& obstacle_db, Point centre,
	 int nbCell, bool& a_ouvrir, bool& arret, size_t& j);
	 
void check_adherance_horizontalle_verticalle_balle
    (vector<Carre>& obstacle_db, Point centre, int nbCell, size_t& j,
    vector<int>& balle_a_effacer, vector<int>& obs_a_effacer);

void changement_base_ball_obs
	(vector<Carre>& obstacle_db, Point centre, 
     int nbCell, bool& a_ouvrir, bool& arret, size_t& i, size_t& j);
     
void changement_base_ball_obs
	(vector<Carre>& obstacle_db, Point centre, 
     int nbCell, size_t& i, size_t& j, vector<int>& balle_a_effacer, 
     vector<int>& obs_a_effacer);
 
void voie_libre(vector<array<int, 2>>&); 

void voie_libre_alignes(vector<array<int, 2>>& targets,
					    vector<Carre>& obs, size_t i,
					    double& x_a, double& y_a,
					    double& x_b, double& y_b);
					   
void voie_libre_non_alignes_haut(vector<array<int, 2>>&, vector<Carre>&,
								size_t, double&, double&, double&, 
								double&);
						
void voie_libre_non_alignes_bas(vector<array<int, 2>>&, vector<Carre>&,
								size_t, double&, double&, double&, 
								double&);
 
void dijkstra_algorithm(size_t, int, matrice&, int&, int&, bool&); 
     
void create_balls(vector<array<int, 2>>&);

void plus_court_chemin(vector<array<int, 2>>&, bool&);

void distances_initiales(matrice& , int&, int&);

void ajout_obstacles(matrice&, vector<Carre>&, int&, int&);

void obstacles_adjacents(matrice&, vector<Carre>&, int&, int&);

void distances_initiales_coins(matrice&, int&, int&);
						       
void distances_initiales_cotes(matrice&, int&, int&);

void check_collisions_balles(int);

void lecture_error_check(char *nom_fichier, bool& a_ouvrir, bool& arret)
{
	string ligne;
	ifstream fichier(nom_fichier);
	if(!fichier.fail())
	{
		while(getline(fichier >> ws, ligne))
		{
			if(ligne[0]=='#') continue;
			if(!arret)
			{
				delegation_de_lecture(ligne, a_ouvrir, arret);
			}
		}
		etat = NB_CELLULES;
		i = 0; 
		total = 0; 
		nbCell = 0;
	} 
	collisions(a_ouvrir, arret);
	if(!a_ouvrir)
	cout << FILE_READING_SUCCESS << endl;
}

void lecture_error_check(string nom_fichier, bool& a_ouvrir,bool& arret)
{
	string ligne;
	ifstream fichier(nom_fichier);
	if(!fichier.fail())
	{
		while(getline(fichier >> ws, ligne))
		{
			if(ligne[0]=='#') continue;
			if(!arret) delegation_de_lecture(ligne, a_ouvrir, arret);
		}
		etat = NB_CELLULES;
		i = 0; 
		total = 0; 
		nbCell = 0;
	} 
	collisions(a_ouvrir, arret);
	if(!a_ouvrir)
	cout << FILE_READING_SUCCESS << endl;
}

void delegation_de_lecture(string ligne, 
						   bool& a_ouvrir, bool& arret)
{
	istringstream data(ligne);
	double x(0), y(0);
	int nb_touch(0), count(0);
	int lgn(0), col(0);
	double inclinaison(0);
	
	switch(etat)
	{
	case NB_CELLULES:
		data >> nbCell;
		set_static_nbCell(nbCell);
		etat = NB_PLAYERS;
		break;
		
	case NB_PLAYERS:
		if(!(data >> total)) exit(EXIT_FAILURE);
		else i=0; 
		if (total==0) etat=NB_OBSTACLES; else etat=PLAYERS; 
		break;
		
	case PLAYERS:
		if(!(data >> x >> y >> nb_touch >> count)) exit(EXIT_FAILURE); 
		else ++i;
		if (i==total) etat=NB_OBSTACLES; 
		update_player_db(x, y, nb_touch, count,
						 nbCell, i, a_ouvrir, arret);
		break;
		
	case NB_OBSTACLES:
		if(!(data >> total)) exit(EXIT_FAILURE);
		else i=0;
		if (total==0) etat=NB_BALLS; else etat=OBSTACLES; 
		break;
		
	case OBSTACLES:
		if(!(data >> lgn >> col)) exit(EXIT_FAILURE);
		else ++i;
		if(i==total) etat=NB_BALLS;
		update_obstacle_db(lgn, col, nbCell, i, a_ouvrir, arret);
		break;
		
	case NB_BALLS:
		if(!(data >> total)) exit(EXIT_FAILURE);
		else i=0;
		if (total==0) etat=FIN; else etat=BALLS; 
		break;
		
	case BALLS:
		if(!(data >> x >> y >> inclinaison)) exit(EXIT_FAILURE);
		else ++i;
		if(i == total) etat = FIN;
		update_balls_db(x, y, inclinaison, nbCell, i, a_ouvrir, arret); 
		break;
		
	case FIN:
		break;
	default: exit(EXIT_FAILURE);
	}	 
}	

void collisions(bool& a_ouvrir, bool& arret)
{
	vector<Carre> obstacle_db;
	vector<Player> player_db;
	vector<Ball> balls_db;
	copie_elements_jeu(player_db, obstacle_db, balls_db);
	if(!arret)
	collis_joueur_joueur(player_db, get_static_nbCell(), 
						 a_ouvrir, arret);
	for(size_t i(0); i < obstacle_db.size(); ++i)
	{
		if(!arret)
		superposition_obstacle(obstacle_db[i].lgn, obstacle_db[i].col, 
							   a_ouvrir, arret, i);
		if(!arret)
		collis_play_obs(obstacle_db[i].lgn, obstacle_db[i].col, 
						player_db, get_static_nbCell(),
					    a_ouvrir, arret, i);
	}
	for(size_t i(0); i < balls_db.size(); ++i)
	{
		if(!arret)
		collis_ball_ball(balls_db[i].get_cercle().centre, 
						 get_static_nbCell(), a_ouvrir, arret, i);
		if(!arret)
		collis_joueur_ball(balls_db[i].get_cercle().centre, player_db, 
						   get_static_nbCell(), a_ouvrir, arret, i);
		if(!arret)
		collis_ball_obs(obstacle_db, balls_db[i].get_cercle().centre, 
						get_static_nbCell(), a_ouvrir, arret, i);
	}
}

//Pour les deux fonctions suivantes: touts les blocs de codes ne sont
//que des changements de base convertissant les lignes et colonnes des
//obstacles en coordonnees cartesiennes

void collis_play_obs(int& lgn, int& col, vector<Player>& player_db, 
					 int nbCell, bool& a_ouvrir, bool& arret, size_t& i)
{	
	double marge_lecture = (COEF_MARGE_JEU/2) * (SIDE/nbCell);
	check_adherance_horizontalle_verticalle_player
	(lgn, col, player_db, nbCell, a_ouvrir, arret, i);											   
	for(size_t j(0); j < player_db.size(); ++j) 
	{
		if(!arret)
		{
			if ((distance(player_db[j].get_cercle().centre, 
				(-DIM_MAX+((col*SIDE)/nbCell)),
				(DIM_MAX-(((lgn+1)*SIDE)/nbCell))) <
				marge_lecture+(COEF_RAYON_JOUEUR*(SIDE/nbCell)))
			or (distance(player_db[j].get_cercle().centre,
				(-DIM_MAX+(((col+1)*SIDE)/nbCell)),
				(DIM_MAX-(((lgn+1)*SIDE)/nbCell))) < 
				marge_lecture+(COEF_RAYON_JOUEUR*(SIDE/nbCell)))
			or (distance(player_db[j].get_cercle().centre,
				(-DIM_MAX+ (((col+1)*SIDE)/nbCell)),
				(DIM_MAX - (((lgn)*SIDE)/nbCell))) <
				marge_lecture+(COEF_RAYON_JOUEUR*(SIDE/nbCell)))
			or (distance(player_db[j].get_cercle().centre,
				(-DIM_MAX+((col*SIDE)/nbCell)),
				(DIM_MAX-(((lgn)*SIDE)/nbCell)))   <
				marge_lecture+(COEF_RAYON_JOUEUR*(SIDE/nbCell))) )
			{  
				cout << COLL_OBST_PLAYER(i+1, j+1) << endl;
				arret = true;
				if(!a_ouvrir) exit(EXIT_FAILURE);
			}
		}
	}
}		

bool collis_play_obs(int nbCell, double& x, double& y)
{	
	bool colli(false);
	vector<Carre> obs;
	copie_obstacle_db(obs);
	double mj = (COEF_MARGE_JEU) * (SIDE/nbCell);
	check_adherance_horizontalle_verticalle_player(obs, x, y, nbCell,
												   colli);											   
	
	for(size_t j(0); j < obs.size(); ++j) 
	{
		if ((distance(x, y, 
			(-DIM_MAX+((obs[j].col*SIDE)/nbCell)),
			(DIM_MAX-(((obs[j].lgn+1)*SIDE)/nbCell))) <
			mj+(COEF_RAYON_JOUEUR*(SIDE/nbCell)))
		or (distance(x, y,
			(-DIM_MAX+(((obs[j].col+1)*SIDE)/nbCell)),
			(DIM_MAX-(((obs[j].lgn+1)*SIDE)/nbCell))) < 
			mj+(COEF_RAYON_JOUEUR*(SIDE/nbCell)))
		or (distance(x, y,
			(-DIM_MAX+ (((obs[j].col+1)*SIDE)/nbCell)),
			(DIM_MAX - (((obs[j].lgn)*SIDE)/nbCell))) <
			mj+(COEF_RAYON_JOUEUR*(SIDE/nbCell)))
		or (distance(x, y,
			(-DIM_MAX+((obs[j].col*SIDE)/nbCell)),
			(DIM_MAX-(((obs[j].lgn)*SIDE)/nbCell)))   <
			mj+(COEF_RAYON_JOUEUR*(SIDE/nbCell))) )
		{  
			colli = true;
		}
	}
	return colli;
}										
				
void collis_ball_obs(vector<Carre>& obstacle_db, Point centre, 
					 int nbCell, bool& a_ouvrir, bool& arret, size_t& j)
{
	double marge_lecture = (COEF_MARGE_JEU/2) * (SIDE/nbCell);
	check_adherance_horizontalle_verticalle_balle
	(obstacle_db, centre, nbCell, a_ouvrir, arret, j);
	for(size_t i(0); i < obstacle_db.size(); ++i) 
	{			  
		if(!arret)
		{
			if ((distance(centre, 
			   (-DIM_MAX+ ((obstacle_db[i].col*SIDE)/nbCell)),
			   (DIM_MAX - (((obstacle_db[i].lgn+1)*SIDE)/nbCell))) <
			   marge_lecture + (COEF_RAYON_BALLE * (SIDE/nbCell)))
			   or(distance(centre,
			   (-DIM_MAX+ (((obstacle_db[i].col+1)*SIDE)/nbCell)),
			   (DIM_MAX - (((obstacle_db[i].lgn+1)*SIDE)/nbCell)))<
			   marge_lecture + (COEF_RAYON_BALLE * (SIDE/nbCell)))
			   or(distance(centre,
			   (-DIM_MAX+ (((obstacle_db[i].col+1)*SIDE)/nbCell)),
			   (DIM_MAX - (((obstacle_db[i].lgn)*SIDE)/nbCell)))  <
			   marge_lecture + (COEF_RAYON_BALLE * (SIDE/nbCell)))
			   or(distance(centre,
			   (-DIM_MAX+ ((obstacle_db[i].col*SIDE)/nbCell)),
			   (DIM_MAX - (((obstacle_db[i].lgn)*SIDE)/nbCell)))  <
			   marge_lecture + (COEF_RAYON_BALLE*(SIDE/nbCell))))
			{
				cout << COLL_BALL_OBSTACLE(j+1) << endl;
				arret = true;
				if(!a_ouvrir) exit(EXIT_FAILURE);
			}
		}
	}
}		

void collis_ball_obs(vector<Carre>& obs, vector<Ball>& balles, 
					 int& nbCell)
{
	vector<int> balle_a_effacer(balles.size(), 0);
	vector<int> obs_a_effacer(obs.size(), 0);
	double mj = (COEF_MARGE_JEU) * (SIDE/nbCell);
	for(size_t j(0); j < balles.size(); ++j)
	{
		Point centre(balles[j].get_cercle().centre);
		check_adherance_horizontalle_verticalle_balle
		(obs, centre, nbCell, j , balle_a_effacer, obs_a_effacer);
		for(size_t i(0); i < obs.size(); ++i) 
		{			  
			if ((distance(centre, 
			(-DIM_MAX+ ((obs[i].col*SIDE)/nbCell)),
			(DIM_MAX - (((obs[i].lgn+1)*SIDE)/nbCell))) <
			mj + (COEF_RAYON_BALLE * (SIDE/nbCell)))
			or(distance(centre,
			(-DIM_MAX+ (((obs[i].col+1)*SIDE)/nbCell)),
			(DIM_MAX - (((obs[i].lgn+1)*SIDE)/nbCell)))<
			mj + (COEF_RAYON_BALLE * (SIDE/nbCell)))
			or(distance(centre,
			(-DIM_MAX+ (((obs[i].col+1)*SIDE)/nbCell)),
			(DIM_MAX - (((obs[i].lgn)*SIDE)/nbCell)))  <
			mj + (COEF_RAYON_BALLE * (SIDE/nbCell)))
			or(distance(centre,
			(-DIM_MAX+ ((obs[i].col*SIDE)/nbCell)),
			(DIM_MAX - (((obs[i].lgn)*SIDE)/nbCell)))  <
			mj + (COEF_RAYON_BALLE*(SIDE/nbCell))))
			{
				balle_a_effacer[j] = 1;
				obs_a_effacer[i] = 1;
				initialisee = false;
			}
		}	
	}
	effacer_balles(balle_a_effacer);
	effacer_obstacles(obs_a_effacer);
}						

void collis_joueur_ball(Point centre_balle, vector<Player>& player_db,
						int nbCell, bool& a_ouvrir, bool& arret, 
						size_t& j)		
{			
	double marge_lecture = (COEF_MARGE_JEU/2) * (SIDE/nbCell);
	for(size_t i(0); i < player_db.size(); ++i) 
	{
		if(!arret)
		{
			if(distance(player_db[i].get_cercle().centre ,centre_balle)<
		      (COEF_RAYON_BALLE * (SIDE/nbCell))+
		      (COEF_RAYON_JOUEUR * (SIDE/nbCell)) + marge_lecture) 
		    {
				cout << PLAYER_BALL_COLLISION(i+1, j+1) << endl;
				arret = true;
				if(!a_ouvrir) exit(EXIT_FAILURE);
			}
		}
	}				
}

void collis_joueur_ball(int& nbCell, vector<Player>& players, 
						vector<Ball>& balles)		
{			
	double marge_jeu = (COEF_MARGE_JEU) * (SIDE/nbCell);
	for(size_t i(0); i < players.size(); ++i) 
	{
		for(size_t j(0); j < balles.size(); ++j) 
		{
			if(distance(players[i].get_cercle().centre ,
			balles[j].get_cercle().centre) <
			(COEF_RAYON_BALLE * (SIDE/nbCell))+
			(COEF_RAYON_JOUEUR * (SIDE/nbCell)) + marge_jeu) 
			{
				hurt_target(i);
				erase_ball(j);
			}
		}
	}				
}				 				 
		
//Cette fonction verifie l'appartenance du centre du joueur a l'espace
//entre les abscisses de l'obstacle et de meme pour les ordonnees.
//Le cas des quatre sommets est traite en dehors de cett fonction.
void check_adherance_horizontalle_verticalle_player
    (int& lgn, int&col , vector<Player>& player_db, 
    int nbCell, bool& a_ouvrir, bool& arret, size_t& i)
{	
	for(size_t j(0); j < player_db.size(); ++j) 
	{
		if(!arret) 
		{
			changement_base_player_obs(lgn, col, player_db, 
			                         nbCell, a_ouvrir, arret, i, j);
			                         		
			if((player_db[j].get_cercle().centre.y < 
			(DIM_MAX-(lgn )*(SIDE/nbCell))) 
			and(player_db[j].get_cercle().centre.y  > 
			(DIM_MAX-(lgn +1)*(SIDE/nbCell))))
			{ 
				if((player_db[j].get_cercle().centre.x <
				  (-DIM_MAX+(COEF_RAYON_JOUEUR*(SIDE/nbCell))+
				  (col +1)*(SIDE/nbCell))) 
				  and(player_db[j].get_cercle().centre.x  >
				  (-DIM_MAX-(COEF_RAYON_JOUEUR*(SIDE/nbCell))+
			      (col)*(SIDE/nbCell))))    
				{ 
					cout << COLL_OBST_PLAYER(i+1, j+1) << endl;
					arret = true;
					if(!a_ouvrir) exit(EXIT_FAILURE);
				}
			}
		}
	}	
}

void check_adherance_horizontalle_verticalle_player
    (vector<Carre>& obs, double& x, double& y, int nbCell, 
    bool& colli)
{	
	for(size_t j(0); j < obs.size(); ++j) 
	{
		changement_base_player_obs(obs, nbCell, x, y, colli, j);
		if((y < (DIM_MAX-(obs[j].lgn )*(SIDE/nbCell))) 
		and(y > (DIM_MAX-(obs[j].lgn +1)*(SIDE/nbCell))))
		{ 
			if((x <(-DIM_MAX+(COEF_RAYON_JOUEUR*(SIDE/nbCell))+
			(obs[j].col +1)*(SIDE/nbCell))) 
			and(x > (-DIM_MAX-(COEF_RAYON_JOUEUR*(SIDE/nbCell))+
		    (obs[j].col)*(SIDE/nbCell))))    
			{ 
				colli = true;
			}
		}
	}
}

void changement_base_player_obs
    (int& lgn, int& col, vector<Player>& player_db, 
     int nbCell, bool& a_ouvrir, bool& arret, size_t& i, size_t& j)
{
	if((player_db[j].get_cercle().centre.x >
	  (-DIM_MAX +(col )*(SIDE/nbCell))) 
	  and(player_db[j].get_cercle().centre.x  < 
	  (-DIM_MAX +(col +1)*(SIDE/nbCell))))
	{
		if((player_db[j].get_cercle().centre.y  >
		(DIM_MAX - (COEF_RAYON_JOUEUR * (SIDE/nbCell))-
   	    (lgn +1)*(SIDE/nbCell) ) ) 
	    and((player_db[j].get_cercle().centre.y) <
		(DIM_MAX + (COEF_RAYON_JOUEUR * (SIDE/nbCell))-
		((lgn)*(SIDE/nbCell)) )) )
		{ 
			cout << COLL_OBST_PLAYER(i+1, j+1) << endl;
			arret = true;
			if(!a_ouvrir) exit(EXIT_FAILURE);
		}	
	}		
}

void changement_base_player_obs(vector<Carre>& obs, int nbCell, 
								double& x, double& y, 
								bool& colli, size_t& j)
{
	if((x > (-DIM_MAX +(obs[j].col )*(SIDE/nbCell))) 
	  and(x < (-DIM_MAX +(obs[j].col +1)*(SIDE/nbCell))))
	{
		if((y > (DIM_MAX - (COEF_RAYON_JOUEUR * (SIDE/nbCell))-
   	    (obs[j].lgn +1)*(SIDE/nbCell) ) ) 
	    and(y < (DIM_MAX + (COEF_RAYON_JOUEUR * (SIDE/nbCell))-
		((obs[j].lgn)*(SIDE/nbCell)) )) )
		{ 
			colli = true;
		}	
	}		
}

//Cette fonction verifie l'appartenance du centre de la balle a l'espace
//entre les abscisses de l'obstacle et de meme pour les ordonnees.
//Le cas des quatre sommets est traite en dehors de cett fonction.
void check_adherance_horizontalle_verticalle_balle
    (vector<Carre>& obstacle_db, Point centre, 
	 int nbCell, bool& a_ouvrir, bool& arret, size_t& j)
{
	for(size_t i(0); i < obstacle_db.size(); ++i) 
	{
		if(!arret)	
		{
			changement_base_ball_obs(obstacle_db, centre, 
								     nbCell, a_ouvrir, arret, i, j);
								     
			if((centre.y <=
			  (DIM_MAX-(obstacle_db[i].lgn )*(SIDE/nbCell))) 
			  and(centre.y >=
			  (DIM_MAX-(obstacle_db[i].lgn +1)*(SIDE/nbCell))))
			{ 
				if((centre.x < 
				  (-DIM_MAX+(COEF_RAYON_BALLE*(SIDE/nbCell))+
				  (obstacle_db[i].col +1)*(SIDE/nbCell))) 
				  and(centre.x >
				  (-DIM_MAX-(COEF_RAYON_BALLE*(SIDE/nbCell))+
				  (obstacle_db[i].col)*(SIDE/nbCell))))	
				{  
					cout <<  COLL_BALL_OBSTACLE(j+1) << endl;
					arret = true;
					if(!a_ouvrir) exit(EXIT_FAILURE);
				}
			}
		}
	}
}

void check_adherance_horizontalle_verticalle_balle
    (vector<Carre>& obs, Point centre, int nbCell, size_t& j,
    vector<int>& balle_a_effacer, vector<int>& obs_a_effacer)
{
	for(size_t i(0); i < obs.size(); ++i) 
	{
		changement_base_ball_obs(obs, centre, nbCell, i, j, 
								balle_a_effacer, obs_a_effacer);						     
		if((centre.y <=
		  (DIM_MAX-(obs[i].lgn )*(SIDE/nbCell))) 
		  and(centre.y >=
		  (DIM_MAX-(obs[i].lgn +1)*(SIDE/nbCell))))
		{ 
			if((centre.x < 
			  (-DIM_MAX+(COEF_RAYON_BALLE*(SIDE/nbCell))+
			  (obs[i].col +1)*(SIDE/nbCell))) 
			  and(centre.x >
			  (-DIM_MAX-(COEF_RAYON_BALLE*(SIDE/nbCell))+
			  (obs[i].col)*(SIDE/nbCell))))	
			{  
				balle_a_effacer[j] = 1;
				obs_a_effacer[i] = 1;
				initialisee = false;
			}
		}
	}
}
	
void changement_base_ball_obs
	(vector<Carre>& obstacle_db, Point centre, 
     int nbCell, bool& a_ouvrir, bool& arret, size_t& i, size_t& j)
{
	if((centre.x >
	  (-DIM_MAX +(obstacle_db[i].col )*(SIDE/nbCell))) 
	  and(centre.x < 
	  (-DIM_MAX+(obstacle_db[i].col +1)*(SIDE/nbCell))))
	{
		if((centre.y >
		  (DIM_MAX-(COEF_RAYON_BALLE*(SIDE/nbCell))-
		  (obstacle_db[i].lgn +1)*(SIDE/nbCell))) 
		  and(centre.y <
		  (DIM_MAX+(COEF_RAYON_BALLE*(SIDE/nbCell))-
		  (obstacle_db[i].lgn )*(SIDE/nbCell))))
		{
			cout << COLL_BALL_OBSTACLE(j+1) << endl; 
			arret = true;
			if(!a_ouvrir) exit(EXIT_FAILURE);
		}
	}	
}

void changement_base_ball_obs
	(vector<Carre>& obs, Point centre, int nbCell, size_t& i, size_t& j,
	vector<int>& balle_a_effacer, vector<int>& obs_a_effacer)
{
	if((centre.x >
	  (-DIM_MAX +(obs[i].col )*(SIDE/nbCell))) 
	  and(centre.x < 
	  (-DIM_MAX+(obs[i].col +1)*(SIDE/nbCell))))
	{
		if((centre.y >
		  (DIM_MAX-(COEF_RAYON_BALLE*(SIDE/nbCell))-
		  (obs[i].lgn +1)*(SIDE/nbCell))) 
		  and(centre.y <
		  (DIM_MAX+(COEF_RAYON_BALLE*(SIDE/nbCell))-
		  (obs[i].lgn )*(SIDE/nbCell))))
		{
			balle_a_effacer[j] = 1;
			obs_a_effacer[i] = 1;
			initialisee = false;
		}
	}	
}
	
void copie_elements_jeu(vector<Player>& copie_players,
						vector<Carre>&  copie_obstacles,
						vector<Ball>&	copie_balls)
{
	copie_player_db(copie_players);
	copie_obstacle_db(copie_obstacles);
	copie_ball_db(copie_balls);
}
	
int copie_nbCell()
{
	return get_static_nbCell();
}		

void get_nb_players(size_t& nb_players)
{
	get_player_db_size(nb_players);
}
	
void get_player_specifications(double& x, double& y, double& rayon, 
							   int& nb_touch, double& count, size_t& i)
{
	access_secret_player_db(x, y, rayon, nb_touch, count, i);
}	

void get_nb_obstacles(size_t& nb_obstacles)
{
	get_obstacles_db_size(nb_obstacles);
}

void get_obstacle_specifications(int& lgn, int& col, size_t& i)
{
	access_secret_obstacles_db(lgn, col, i);
}

void get_nb_balls(size_t& nb_balls)
{
	get_ball_db_size(nb_balls);
}

void get_ball_specifications(double& x, double& y, 
							 double& rayon, size_t& i)
{
	access_secret_ball_db(x, y, rayon, i);
}

void clear_nbCell()
{
	clear_static_nbCell();
}

void clear_player_secret_db()
{
	clear_static_players();
}

void clear_obstacle_secret_db()
{
	clear_static_obstacles();
}
	
void clear_ball_secret_db()
{
	clear_static_balls();
}

void save(string& save_file)
{
	ofstream file;
	file.open(save_file);
	
	vector<Player> copie_players;
	vector<Carre>  copie_obstacles;
	vector<Ball>   copie_balls;
	copie_elements_jeu(copie_players, copie_obstacles, copie_balls);
	file << copie_nbCell() << endl;
	
	file << copie_players.size() << endl;
	for(auto player : copie_players)
	{
		file << player.get_cercle().centre.x << " " 
			 << player.get_cercle().centre.y << " "
			 << player.get_nb_touch() << " "
			 << player.get_count()    << endl;
	}
	
	file << copie_obstacles.size() << endl;
	for(auto obstacle: copie_obstacles)
	{
		file << obstacle.lgn << " " << obstacle.col << endl;
	}
	
	file << copie_balls.size() << endl; 
	for(auto ball : copie_balls)
	{
		file << ball.get_cercle().centre.x << " " 
			 << ball.get_cercle().centre.y << " "
			 << ball.get_inclinaison() << endl;
	}
}

void simulate(bool& endgame, bool& nogame)
{
	vector<array<int, 2>> targets(get_player_db_size());
	if(targets.size() <= 1) 
	{	
		endgame = true;
	}
	double tmp(0),tmp_new(0),x_player_a(0),y_player_a(0),
		   x_player_b(0),y_player_b(0);
		for(size_t i(0); i < targets.size(); ++i)
	{
		access_secret_player_db(x_player_a, y_player_a, i);
		targets[i][0] = -1;
		for(size_t j(0); j < targets.size(); ++j)
		{
			if(!(i==j))
			{
				access_secret_player_db(x_player_b, y_player_b, j);
				tmp_new = distance(x_player_a, y_player_a, 
								  x_player_b, y_player_b);
				if(targets[i][0] == -1 || tmp > tmp_new)
				{
					targets[i][0] = j;
				}
				tmp = tmp_new;
			}
		}
	}
	voie_libre(targets);
	plus_court_chemin(targets, nogame);
	increment_count();
	create_balls(targets);
	update_balls_position(get_static_nbCell());
	check_collisions_balles(get_static_nbCell());
}

void plus_court_chemin(vector<array<int, 2>>& targets ,bool& nogame)
{
	vector<Carre> obs;
	copie_obstacle_db(obs);
	int nbCell(get_static_nbCell());
	int nbCell_carre(nbCell*nbCell);
	static matrice distances(nbCell_carre,
							 vector<double>(8, nbCell_carre));
	if(!initialisee)
	{
		matrice new_dist(nbCell_carre, vector<double>(8, nbCell_carre));
		distances = new_dist;
		distances_initiales(distances, nbCell, nbCell_carre);
		ajout_obstacles(distances, obs, nbCell, nbCell_carre); 
		obstacles_adjacents(distances, obs, nbCell, nbCell_carre);
	}
	for(size_t i(0); i < targets.size(); ++i)
	{
		if(targets[i][1] == 0)
		{
			dijkstra_algorithm(i, targets[i][0], distances, nbCell, 
							   nbCell_carre, nogame);
		}
		else 
		{
			update_player_position(i, targets[i][0], nbCell);
		}
	}
}

void create_balls(vector<array<int, 2>>& targets)
{
    int nbCell(get_static_nbCell());
    double x_a(0), y_a(0), x_b(0), y_b(0), 
		   inclinaison(0);
    
    for (size_t i(0); i < targets.size(); ++i)
    {
        if (access_secret_player_db_count(i) >= MAX_COUNT 
        and targets[i][1] == 1)
        {
            access_secret_player_db(x_a, y_a, i);
			access_secret_player_db(x_b, y_b, targets[i][0]);
            if(distance(x_a, y_a, x_b, y_b) > (2*COEF_RAYON_JOUEUR+
            COEF_MARGE_JEU)*SIDE/nbCell)
            {
				inclinaison = atan((y_b-y_a)/(x_b-x_a));
				if(x_b - x_a < 0)
				{
					inclinaison += M_PI;
				}
				x_a += (cos(inclinaison))*((COEF_RAYON_JOUEUR+
				COEF_RAYON_BALLE+COEF_MARGE_JEU)*(SIDE/nbCell));
				y_a += (sin(inclinaison))*((COEF_RAYON_JOUEUR+
				COEF_RAYON_BALLE+COEF_MARGE_JEU)*(SIDE/nbCell));
				update_balls_db(x_a,y_a, inclinaison, nbCell);
			}
			else
			{
				hurt_target(targets[i][0]);
			}
			reset_count(i);
        }
    }        
} 

void voie_libre(vector<array<int, 2>>& targets)
{
	vector<Carre> copie_obstacles;
	copie_obstacle_db(copie_obstacles);
	double x_player_a(0), y_player_a(0), x_player_b(0), y_player_b(0);
	for(size_t i(0); i < targets.size(); ++i)
	{
		access_secret_player_db(x_player_a, y_player_a, i);
		access_secret_player_db(x_player_b, y_player_b, targets[i][0]);
		targets[i][1] = 1;
		voie_libre_alignes(targets, copie_obstacles, i, x_player_a, 
						   y_player_a, x_player_b, y_player_b);
		if(targets[i][1] == 1)
		{
			voie_libre_non_alignes_haut(targets, copie_obstacles, i,
						x_player_a, y_player_a, x_player_b, y_player_b);
		}
		if(targets[i][1] == 1)
		{
			voie_libre_non_alignes_bas(targets, copie_obstacles, i,
						x_player_a, y_player_a, x_player_b, y_player_b);
		}
	}
}

void voie_libre_alignes(vector<array<int, 2>>& targets,
					    vector<Carre>& obs, size_t i,
					    double& x_player_a, double& y_player_a,
					    double& x_player_b, double& y_player_b)
{
	int nbCell(get_static_nbCell());
	if(x_player_a > x_player_b) 
	{ 
		swap(x_player_a, x_player_b); 
		swap(y_player_a, y_player_b);
	}
	for(size_t j(0); j < obs.size(); ++j)
	{
		if(((x_player_a-x_player_b) == 0)
		and (y_player_a > (DIM_MAX-obs[j].lgn * (SIDE/nbCell))) 
		and (y_player_b < (DIM_MAX-(obs[j].lgn+1) * (SIDE/nbCell))))
		{
			targets[i][1] = 0;
			j = obs.size();
		}
		else if(((x_player_a-x_player_b) == 0)
		and (y_player_b > (DIM_MAX-obs[j].lgn * (SIDE/nbCell)))
		and (y_player_a < (DIM_MAX-(obs[j].lgn+1) * (SIDE/nbCell))))
		{
			targets[i][1] = 0;
			j = obs.size();
		}
	}
}


void voie_libre_non_alignes_haut(vector<array<int, 2>>& targets, 
							    vector<Carre>& obs,size_t i,
							    double& x_a, double& y_a,
							    double& x_b, double& y_b)
{
	double nbCell(get_static_nbCell()),mj(COEF_MARGE_JEU*(SIDE/nbCell));
	double rayon(COEF_RAYON_JOUEUR*SIDE/nbCell);
	double angle(atan((y_b-y_a)/(x_b-x_a)));
	for(size_t j(0); j < obs.size(); ++j)
	{		
		if((x_a-rayon-mj <= (obs[j].col+1)*SIDE/nbCell-DIM_MAX) 
		and((x_b+rayon+mj >= (obs[j].col)*SIDE/nbCell-DIM_MAX)))
		{
			if(((((y_b-y_a)/(x_b-x_a))*((obs[j].col)*(SIDE/nbCell)
			-DIM_MAX-x_a+sin(angle)*(rayon+mj))+y_a+cos(angle)*
			(rayon+mj))< DIM_MAX-(obs[j].lgn)*(SIDE/nbCell))
			and	((((y_b-y_a)/(x_b-x_a))*((obs[j].col+1)*(SIDE/nbCell)
			-DIM_MAX-x_a-sin(angle)*(rayon+mj))+y_a+cos(angle)*
			(rayon+mj))> DIM_MAX-(obs[j].lgn+1)*(SIDE/nbCell)))
			{
				targets[i][1] = 0;
				j = obs.size();			
			}
			else if(((((y_b-y_a)/(x_b-x_a))*((obs[j].col)*(SIDE/nbCell)
			-DIM_MAX-x_a-cos(angle)*(rayon+mj))+y_a-sin(angle)*
			(rayon+mj))> DIM_MAX-(obs[j].lgn+1)*(SIDE/nbCell))
			and	((((y_b-y_a)/(x_b-x_a))*((obs[j].col+1)*(SIDE/nbCell)
			-DIM_MAX-x_a-cos(angle)*(rayon+mj))+y_a-sin(angle)*
			(rayon+mj))< DIM_MAX-(obs[j].lgn)*(SIDE/nbCell)))
			{		    
				targets[i][1] = 0;
				j = obs.size();
			}
		}
	}
}

void voie_libre_non_alignes_bas(vector<array<int, 2>>& targets, 
							    vector<Carre>& obs,size_t i,
							    double& x_a, double& y_a,
							    double& x_b, double& y_b)
{
	double nbCell(get_static_nbCell()),mj(COEF_MARGE_JEU*(SIDE/nbCell));
	double rayon(COEF_RAYON_JOUEUR*SIDE/nbCell);
	double angle(atan((y_b-y_a)/(x_b-x_a)));
	for(size_t j(0); j < obs.size(); ++j)
	{		
		if((x_a-rayon-mj <=(obs[j].col+1)*SIDE/nbCell-DIM_MAX) 
		and((x_b+rayon+mj >= (obs[j].col)*SIDE/nbCell-DIM_MAX)))
		{
			if(((((y_b-y_a)/(x_b-x_a))*((obs[j].col)*(SIDE/nbCell)
			-DIM_MAX-x_a-cos(angle)*(rayon+mj))+y_a-sin(angle)*
			(rayon+mj))< DIM_MAX-(obs[j].lgn)*(SIDE/nbCell))
			and	((((y_b-y_a)/(x_b-x_a))*((obs[j].col+1)*(SIDE/nbCell)
			-DIM_MAX-x_a-cos(angle)*(rayon+mj))+y_a-sin(angle)*
			(rayon+mj))> DIM_MAX-(obs[j].lgn+1)*(SIDE/nbCell)))	   
			{	 
				targets[i][1] = 0;
				j = obs.size();
			}	
			else if(((((y_b-y_a)/(x_b-x_a))*((obs[j].col)*(SIDE/nbCell)
			-DIM_MAX-x_a+cos(angle)*(rayon+mj))+y_a-cos(angle)*
			(rayon+mj))> DIM_MAX-(obs[j].lgn+1)*(SIDE/nbCell))
			and	((((y_b-y_a)/(x_b-x_a))*((obs[j].col+1)*(SIDE/nbCell)
			-DIM_MAX-x_a+cos(angle)*(rayon+mj))+y_a-cos(angle)*
			(rayon+mj))< DIM_MAX-(obs[j].lgn)*(SIDE/nbCell)))
			{
				targets[i][1] = 0;
				j = obs.size();
			}
		}
	}
}

void distances_initiales(matrice& distances, 
						 int& nbCell, int& nbCell_carre)
{
	distances_initiales_coins(distances, nbCell, nbCell_carre);
	distances_initiales_cotes(distances, nbCell, nbCell_carre);
	
	for(int i(1); i < nbCell-1; ++i)
	{
		for(int j(1); j < nbCell-1; ++j)
		{	
			distances[nbCell*(i)+j][0] = sqrt(2);
			distances[nbCell*(i)+j][1] = 1;
			distances[nbCell*(i)+j][2] = sqrt(2);
			distances[nbCell*(i)+j][3] = 1;
			distances[nbCell*(i)+j][4] = sqrt(2);
			distances[nbCell*(i)+j][5] = 1;
			distances[nbCell*(i)+j][6] = sqrt(2);
			distances[nbCell*(i)+j][7] = 1;
		}
	}
}

void distances_initiales_coins(matrice& distances, 
						       int& nbCell, int& nbCell_carre)
{
	if(nbCell > 0)
	{
		distances[0][3] = 1;
		distances[0][4] = sqrt(2);
		distances[0][5] = 1;
		distances[nbCell-1][5] = 1;
		distances[nbCell-1][6] = sqrt(2);
		distances[nbCell-1][7] = 1;
		distances[nbCell*(nbCell-1)][1] = 1;
		distances[nbCell*(nbCell-1)][2] = sqrt(2); 
		distances[nbCell*(nbCell-1)][3] = 1;
		distances[nbCell_carre-1][0] = 1;
		distances[nbCell_carre-1][1] = sqrt(2);
		distances[nbCell_carre-1][7] = 1;
	}
}

void distances_initiales_cotes(matrice& distances, 
						       int& nbCell, int& nbCell_carre)
{
	for(int i(0); i < nbCell-2; ++i)
	{
		distances[i+1][3] = 1;
		distances[i+1][4] = sqrt(2);
		distances[i+1][5] = 1;
		distances[i+1][6] = sqrt(2);
		distances[i+1][7] = 1;
		
		distances[(2+i)*nbCell-1][0] = sqrt(2);
		distances[(2+i)*nbCell-1][1] = 1;
		distances[(2+i)*nbCell-1][5] = 1;
		distances[(2+i)*nbCell-1][6] = sqrt(2);
		distances[(2+i)*nbCell-1][7] = 1;
		
		distances[nbCell*(nbCell-1)+1+i][0] = sqrt(2);
		distances[nbCell*(nbCell-1)+1+i][1] = 1;
		distances[nbCell*(nbCell-1)+1+i][2] = sqrt(2);
		distances[nbCell*(nbCell-1)+1+i][3] = 1;
		distances[nbCell*(nbCell-1)+1+i][7] = 1;
		
		distances[nbCell*(i+1)][1] = 1; 
		distances[nbCell*(i+1)][2] = sqrt(2);
		distances[nbCell*(i+1)][3] = 1;
		distances[nbCell*(i+1)][4] = sqrt(2);
		distances[nbCell*(i+1)][5] = 1;
	}
}

void ajout_obstacles(matrice& distances, vector<Carre>& obs,
					 int& nbCell, int& nbC_c)
{
	for(size_t i(0); i < obs.size(); ++i)
	{
		if(obs[i].lgn == 0 and obs[i].col == 0)
		{
			distances[obs[i].col+1+(obs[i].lgn+1)*nbCell][0] = nbC_c;
			distances[obs[i].col+1+(obs[i].lgn)*nbCell][7] = nbC_c;
			distances[obs[i].col+(obs[i].lgn+1)*nbCell][1] = nbC_c;
			if(distances[obs[i].col+1+(obs[i].lgn)*nbCell][6] !=
			nbC_c)
				distances[obs[i].col+1+(obs[i].lgn)*nbCell][6]=2.1;	
			if(distances[obs[i].col+(obs[i].lgn+1)*nbCell][2] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn+1)*nbCell][2]=2.1;
		}
		else if(obs[i].lgn == 0 and obs[i].col == nbCell-1)
		{
			distances[obs[i].col-1+(obs[i].lgn+1)*nbCell][2] = nbC_c;
			distances[obs[i].col-1+(obs[i].lgn)*nbCell][3] = nbC_c;
			distances[obs[i].col+(obs[i].lgn+1)*nbCell][1] = nbC_c;
			if(distances[obs[i].col-1+(obs[i].lgn)*nbCell][4] !=
			nbC_c)
				distances[obs[i].col-1+(obs[i].lgn)*nbCell][4]=2.1;
			if(distances[obs[i].col-1+(obs[i].lgn)*nbCell][4] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn+1)*nbCell][0]=2.1;
		}
		else if(obs[i].lgn == nbCell-1 and obs[i].col == 0)
		{
			distances[obs[i].col+(obs[i].lgn-1)*nbCell][5] = nbC_c;
			distances[obs[i].col+1+(obs[i].lgn-1)*nbCell][6] = nbC_c;
			distances[obs[i].col+1+(obs[i].lgn)*nbCell][7] = nbC_c;
			if(distances[obs[i].col-1+(obs[i].lgn)*nbCell][4] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn-1)*nbCell][4]=2.1;
			if(distances[obs[i].col-1+(obs[i].lgn)*nbCell][4] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn+1)*nbCell][0]=2.1;
		}
		else if(obs[i].lgn == nbCell-1 and obs[i].col == nbCell-1)
		{
			distances[obs[i].col-1+(obs[i].lgn)*nbCell][3] = nbC_c;
			distances[obs[i].col-1+(obs[i].lgn-1)*nbCell][4] = nbC_c;
			distances[obs[i].col+(obs[i].lgn-1)*nbCell][5] = nbC_c;
			if(distances[obs[i].col-1+(obs[i].lgn)*nbCell][2] !=
			nbC_c)
				distances[obs[i].col-1+(obs[i].lgn)*nbCell][2]=2.1;
			if(distances[obs[i].col+(obs[i].lgn-1)*nbCell][6] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn-1)*nbCell][6]=2.1;
		}
		else if(obs[i].lgn == 0)
		{
			distances[obs[i].col-1+(obs[i].lgn)*nbCell][3] = nbC_c;
			distances[obs[i].col-1+(obs[i].lgn+1)*nbCell][2] = nbC_c;
			distances[obs[i].col+(obs[i].lgn+1)*nbCell][1] = nbC_c;
			distances[obs[i].col+1+(obs[i].lgn+1)*nbCell][0] = nbC_c;
			distances[obs[i].col+1+(obs[i].lgn)*nbCell][7] = nbC_c;
			if(distances[obs[i].col-1+(obs[i].lgn)*nbCell][4] !=
			nbC_c)
				distances[obs[i].col-1+(obs[i].lgn)*nbCell][4]=2.1;
			if(distances[obs[i].col+(obs[i].lgn+1)*nbCell][0] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn+1)*nbCell][0]=2.1;
			if(distances[obs[i].col+(obs[i].lgn+1)*nbCell][2] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn+1)*nbCell][2]=2.1;
			if(distances[obs[i].col+1+(obs[i].lgn)*nbCell][6] !=
			nbC_c)
				distances[obs[i].col+1+(obs[i].lgn)*nbCell][6]=2.1;
		}
		else if(obs[i].col == nbCell-1)
		{
			distances[obs[i].col+(obs[i].lgn+1)*nbCell][1] = nbC_c;
			distances[obs[i].col-1+(obs[i].lgn+1)*nbCell][2] = nbC_c;
			distances[obs[i].col-1+(obs[i].lgn)*nbCell][3] = nbC_c;
			distances[obs[i].col-1+(obs[i].lgn-1)*nbCell][4] = nbC_c;
			distances[obs[i].col+(obs[i].lgn-1)*nbCell][5] = nbC_c;
			if(distances[obs[i].col+(obs[i].lgn-1)*nbCell][6] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn-1)*nbCell][6]=2.1;
			if(distances[obs[i].col-1+(obs[i].lgn)*nbCell][2] !=
			nbC_c)
				distances[obs[i].col-1+(obs[i].lgn)*nbCell][2]=2.1;
			if(distances[obs[i].col-1+(obs[i].lgn)*nbCell][4] !=
			nbC_c)
				distances[obs[i].col-1+(obs[i].lgn)*nbCell][4]=2.1;
			if(distances[obs[i].col+(obs[i].lgn+1)*nbCell][0] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn+1)*nbCell][0]=2.1;
		}
		else if(obs[i].lgn == nbCell-1)
		{
			distances[obs[i].col-1+(obs[i].lgn)*nbCell][3] = nbC_c;
			distances[obs[i].col-1+(obs[i].lgn-1)*nbCell][4] = nbC_c;
			distances[obs[i].col+(obs[i].lgn-1)*nbCell][5] = nbC_c;
			distances[obs[i].col+1+(obs[i].lgn-1)*nbCell][6] = nbC_c;
			distances[obs[i].col+1+(obs[i].lgn)*nbCell][7] = nbC_c;
			if(distances[obs[i].col-1+(obs[i].lgn)*nbCell][2] !=
			nbC_c)
				distances[obs[i].col-1+(obs[i].lgn)*nbCell][2]=2.1;
			if(distances[obs[i].col+(obs[i].lgn-1)*nbCell][4] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn-1)*nbCell][4]=2.1;
			if(distances[obs[i].col+(obs[i].lgn-1)*nbCell][6] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn-1)*nbCell][6]=2.1;
			if(distances[obs[i].col+1+(obs[i].lgn)*nbCell][0] !=
			nbC_c)
				distances[obs[i].col+1+(obs[i].lgn)*nbCell][0]=2.1;
		}
		else if(obs[i].col == 0)
		{
			distances[obs[i].col+(obs[i].lgn+1)*nbCell][1] = nbC_c;
			distances[obs[i].col-1+(obs[i].lgn+1)*nbCell][2] = nbC_c;
			distances[obs[i].col-1+(obs[i].lgn)*nbCell][3] = nbC_c;
			distances[obs[i].col-1+(obs[i].lgn-1)*nbCell][4] = nbC_c;
			distances[obs[i].col+(obs[i].lgn-1)*nbCell][5] = nbC_c;
			if(distances[obs[i].col+(obs[i].lgn-1)*nbCell][4] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn-1)*nbCell][4]=2.1;
			if(distances[obs[i].col+1+(obs[i].lgn)*nbCell][6] !=
			nbC_c)
				distances[obs[i].col+1+(obs[i].lgn)*nbCell][6]=2.1;
			if(distances[obs[i].col+1+(obs[i].lgn)*nbCell][0] !=
			nbC_c)
				distances[obs[i].col+1+(obs[i].lgn)*nbCell][0]=2.1;
			if(distances[obs[i].col+(obs[i].lgn+1)*nbCell][2] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn+1)*nbCell][2]=2.1;
		}
		else
		{
			distances[obs[i].col-1+(obs[i].lgn-1)*nbCell][4] = nbC_c;
			distances[obs[i].col+1+(obs[i].lgn-1)*nbCell][6] = nbC_c;
			distances[obs[i].col+1+(obs[i].lgn+1)*nbCell][0] = nbC_c;
			distances[obs[i].col-1+(obs[i].lgn+1)*nbCell][2] = nbC_c;
			distances[obs[i].col+(obs[i].lgn-1)*nbCell][5] = nbC_c;
			distances[obs[i].col+1+(obs[i].lgn)*nbCell][7] = nbC_c;
			distances[obs[i].col+(obs[i].lgn+1)*nbCell][1] = nbC_c;
			distances[obs[i].col-1+(obs[i].lgn)*nbCell][3] = nbC_c;
			if(distances[obs[i].col+(obs[i].lgn-1)*nbCell][4] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn-1)*nbCell][4]=2.1;
			if(distances[obs[i].col+(obs[i].lgn-1)*nbCell][6] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn-1)*nbCell][6]=2.1;
			if(distances[obs[i].col+1+(obs[i].lgn)*nbCell][6] !=
			nbC_c)
				distances[obs[i].col+1+(obs[i].lgn)*nbCell][6]=2.1;
			if(distances[obs[i].col+1+(obs[i].lgn)*nbCell][0] !=
			nbC_c)
				distances[obs[i].col+1+(obs[i].lgn)*nbCell][0]=2.1;
			if(distances[obs[i].col+(obs[i].lgn+1)*nbCell][0] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn+1)*nbCell][0]=2.1;
			if(distances[obs[i].col+(obs[i].lgn+1)*nbCell][2] !=
			nbC_c)
				distances[obs[i].col+(obs[i].lgn+1)*nbCell][2]=2.1;
			if(distances[obs[i].col-1+(obs[i].lgn)*nbCell][2] !=
			nbC_c)
				distances[obs[i].col-1+(obs[i].lgn)*nbCell][2]=2.1;
			if(distances[obs[i].col-1+(obs[i].lgn)*nbCell][4] !=
			nbC_c)
				distances[obs[i].col-1+(obs[i].lgn)*nbCell][4]=2.1;
		}
	}
	for(size_t i(0); i < obs.size(); ++i)
	{
		for(size_t j(0); j < 8; ++j)
		{
			distances[obs[i].col+obs[i].lgn*nbCell][j] = nbC_c;
		}
	}
}

void obstacles_adjacents(matrice& distances, vector<Carre>& obs, 
						 int& nbCell, int& nbC_c)
{
	for(size_t i(0); i < obs.size(); ++i)
	{
		for(size_t j(i+1); j < obs.size(); ++j)
		{
			if(obs[i].lgn == obs[j].lgn-1)
			{
				if(obs[i].col == obs[j].col-1)
				{
					distances[obs[i].col+(obs[j].lgn)*nbCell][2] =nbC_c;
					distances[obs[j].col+(obs[i].lgn)*nbCell][6] =nbC_c;
				}
				if(obs[i].col == obs[j].col+1)
				{
					distances[obs[j].col+(obs[i].lgn)*nbCell][4] =nbC_c;
					distances[obs[j].col+(obs[i].lgn)*nbCell][0] =nbC_c;
				}
			}
			if(obs[i].lgn == obs[j].lgn+1)
			{
				if(obs[i].col == obs[j].col-1)
				{
					distances[obs[i].col+(obs[j].lgn)*nbCell][4] =nbC_c;
					distances[obs[j].col+(obs[i].lgn)*nbCell][0] =nbC_c;
				}
				if(obs[i].col == obs[j].col+1)
				{
					distances[obs[j].col+(obs[i].lgn)*nbCell][2] =nbC_c;
					distances[obs[j].col+(obs[i].lgn)*nbCell][6] =nbC_c;
				}
			}
		}
	}
	initialisee = true;
}

void dijkstra_algorithm(size_t i, int cible, matrice& dist, 
					    int& nbCell, int& nbC_c, bool& nogame)
{
	vector<int> visited_vertices(nbC_c, 0);
	vector<double> target(nbC_c, nbC_c);
	int x_a(0), y_a(0), x_b(0), y_b(0);
	access_secret_player_db(x_a, y_a, nbCell, cible);
	int spot(x_a+y_a*nbCell), targeted_spot(0);
	target[spot] = 0;
	for(int i(0); i < nbC_c; ++i)
	{
		visited_vertices[spot] = 1;
		if(spot == 0)
		{
			if(target[spot+1] > target[spot] + dist[spot][3])
				target[spot+1] = target[spot] + dist[spot][3];
			if(target[spot+nbCell+1] > target[spot] + dist[spot][4])
				target[spot+nbCell+1] = target[spot] + dist[spot][4];
			if(target[spot+nbCell] > target[spot] + dist[spot][5])
				target[spot+nbCell] = target[spot] + dist[spot][5];
		}
		else if(spot == nbCell-1)
		{
			if(target[spot+nbCell] > target[spot] + dist[spot][5])
				target[spot+nbCell] = target[spot] + dist[spot][5];
			if(target[spot+nbCell-1] > target[spot] + dist[spot][6])
				target[spot+nbCell-1] = target[spot] + dist[spot][6];
			if(target[spot-1] > target[spot] + dist[spot][7])
				target[spot-1] = target[spot] + dist[spot][7];
		}
		else if(spot == nbCell*(nbCell-1))
		{
			if(target[spot-nbCell] > target[spot] + dist[spot][1])
				target[spot-nbCell] = target[spot] + dist[spot][1];
			if(target[spot-nbCell+1] > target[spot] + dist[spot][2])
				target[spot-nbCell+1] = target[spot] + dist[spot][2];
			if(target[spot+1] > target[spot] + dist[spot][3])
				target[spot+1] = target[spot] + dist[spot][3];	
		}
		else if(spot == nbC_c-1)
		{
			if(target[spot-nbCell-1] > target[spot] + dist[spot][0])
				target[spot-nbCell-1] = target[spot] + dist[spot][0];
			if(target[spot-nbCell] > target[spot] + dist[spot][1])
				target[spot-nbCell] = target[spot] + dist[spot][1];
			if(target[spot-1] > target[spot] + dist[spot][7])
				target[spot-1] = target[spot] + dist[spot][7];
		}
		else if(spot < nbCell)
		{
			if(target[spot+1] > target[spot] + dist[spot][3])
				target[spot+1] = target[spot] + dist[spot][3];
			if(target[spot+nbCell+1] > target[spot] + dist[spot][4])
				target[spot+nbCell+1] = target[spot] + dist[spot][4];
			if(target[spot+nbCell] > target[spot] + dist[spot][5])
				target[spot+nbCell] = target[spot] + dist[spot][5];
			if(target[spot+nbCell-1] > target[spot] + dist[spot][6])
				target[spot+nbCell-1] = target[spot] + dist[spot][6];
			if(target[spot-1] > target[spot] + dist[spot][7])
				target[spot-1] = target[spot] + dist[spot][7];
		}
		else if(spot%nbCell == nbCell-1)
		{
			if(target[spot-nbCell-1] > target[spot] + dist[spot][0])
				target[spot-nbCell-1] = target[spot] + dist[spot][0];
			if(target[spot-nbCell] > target[spot] + dist[spot][1])
				target[spot-nbCell] = target[spot] + dist[spot][1];
			if(target[spot+nbCell] > target[spot] + dist[spot][5])
				target[spot+nbCell] = target[spot] + dist[spot][5];
			if(target[spot+nbCell-1] > target[spot] + dist[spot][6])
				target[spot+nbCell-1] = target[spot] + dist[spot][6];
			if(target[spot-1] > target[spot] + dist[spot][7])
				target[spot-1] = target[spot] + dist[spot][7];
		}
		else if(spot > nbCell*(nbCell-1))
		{
			if(target[spot-nbCell-1] > target[spot] + dist[spot][0])
				target[spot-nbCell-1] = target[spot] + dist[spot][0];
			if(target[spot-nbCell] > target[spot] + dist[spot][1])
				target[spot-nbCell] = target[spot] + dist[spot][1];
			if(target[spot-nbCell+1] > target[spot] + dist[spot][2])
				target[spot-nbCell+1] = target[spot] + dist[spot][2];
			if(target[spot+1] > target[spot] + dist[spot][3])
				target[spot+1] = target[spot] + dist[spot][3];
			if(target[spot-1] > target[spot] + dist[spot][7])
				target[spot-1] = target[spot] + dist[spot][7];
		}
		else if(spot%nbCell == 0)
		{
			if(target[spot-nbCell] > target[spot] + dist[spot][1])
				target[spot-nbCell] = target[spot] + dist[spot][1];
			if(target[spot-nbCell+1] > target[spot] + dist[spot][2])
				target[spot-nbCell+1] = target[spot] + dist[spot][2];
			if(target[spot+1] > target[spot] + dist[spot][3])
				target[spot+1] = target[spot] + dist[spot][3];
			if(target[spot+nbCell+1] > target[spot] + dist[spot][4])
				target[spot+nbCell+1] = target[spot] + dist[spot][4];
			if(target[spot+nbCell] > target[spot] + dist[spot][5])
				target[spot+nbCell] = target[spot] + dist[spot][5];
		}
		else
		{
			if(target[spot-nbCell-1] > target[spot] + dist[spot][0])
				target[spot-nbCell-1] = target[spot] + dist[spot][0];
			if(target[spot-nbCell] > target[spot] + dist[spot][1])
				target[spot-nbCell] = target[spot] + dist[spot][1];
			if(target[spot-nbCell+1] > target[spot] + dist[spot][2])
				target[spot-nbCell+1] = target[spot] + dist[spot][2];
			if(target[spot+1] > target[spot] + dist[spot][3])
				target[spot+1] = target[spot] + dist[spot][3];
			if(target[spot+nbCell+1] > target[spot] + dist[spot][4])
				target[spot+nbCell+1] = target[spot] + dist[spot][4];
			if(target[spot+nbCell] > target[spot] + dist[spot][5])
				target[spot+nbCell] = target[spot] + dist[spot][5];
			if(target[spot+nbCell-1] > target[spot] + dist[spot][6])
				target[spot+nbCell-1] = target[spot] + dist[spot][6];
			if(target[spot-1] > target[spot] + dist[spot][7])
				target[spot-1] = target[spot] + dist[spot][7];
		}
		int j(0);
		while(visited_vertices[j] == 1)
		{
			++j;
		}
		for(int k(j); k < nbC_c; ++k)
		{
			if(visited_vertices[k] == 0 and k != spot)
			{
				if(target[k] < target[j])
				{
					j = k;
				} 
			}
		}
		spot=j;
	}
	access_secret_player_db(x_b, y_b, nbCell, i);
	int player_spot(x_b+y_b*nbCell);
	int tmp_spot(0);
	if(x_b+y_b == 0)  
	{
		targeted_spot = player_spot+1; tmp_spot = 3;
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot+nbCell+1]+dist[player_spot][4]) 
	    {
			targeted_spot = player_spot+nbCell+1; 
			tmp_spot = 4;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot+nbCell]+dist[player_spot][5])
		{
			targeted_spot = player_spot+nbCell;
		}
	}
	else if(x_b+y_b*nbCell == nbCell-1)
	{
		targeted_spot = player_spot+nbCell; tmp_spot = 5;
		if(target[targeted_spot]+dist[player_spot][tmp_spot] >
		target[player_spot+nbCell-1]+dist[player_spot][6]) 
		{
			targeted_spot = player_spot+nbCell-1; 
			tmp_spot = 6;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] >
		target[player_spot-1]+dist[player_spot][7]) 
			targeted_spot = player_spot-1;
	}
	else if(x_b+y_b*nbCell == nbCell*(nbCell-1))
	{
		targeted_spot = player_spot-nbCell; tmp_spot = 1;
		if(target[targeted_spot]+dist[player_spot][tmp_spot] >
		target[player_spot-nbCell+1]+dist[player_spot][2]) 
		{
			targeted_spot = player_spot-nbCell+1;
			 tmp_spot = 2;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] >
		target[player_spot+1]+dist[player_spot][3])
			targeted_spot = player_spot+1;
	}
	else if(x_b+y_b*nbCell == nbC_c-1)
	{
		targeted_spot = player_spot-1; tmp_spot = 7;
		if(target[targeted_spot]+dist[player_spot][tmp_spot] >
		target[player_spot-nbCell-1]+dist[player_spot][0])
	    {
			targeted_spot = player_spot-nbCell-1;
			tmp_spot = 0;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot-nbCell]+dist[player_spot][1])
			targeted_spot = player_spot-nbCell;
	}
	else if(y_b == 0)
	{
		targeted_spot = player_spot+1; tmp_spot = 3;
		if(target[targeted_spot]+dist[player_spot][tmp_spot] >
		target[player_spot+nbCell+1]+dist[player_spot][4]) 
		{
			targeted_spot = player_spot+nbCell+1;
			tmp_spot = 4;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] >
		target[player_spot+nbCell]+dist[player_spot][5]) 
	    {
			targeted_spot = player_spot+nbCell; 
			tmp_spot = 5;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] >
		target[player_spot+nbCell-1]+dist[player_spot][6]) 
		{
			targeted_spot = player_spot+nbCell-1; 
			tmp_spot = 6;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot-1]+dist[player_spot][7])
			targeted_spot = player_spot-1;
	}
	else if(x_b == nbCell-1)
	{
		targeted_spot = player_spot+nbCell; tmp_spot = 5;
		if(target[targeted_spot]+dist[player_spot][tmp_spot] >
		target[player_spot+nbCell-1]+dist[player_spot][6])
		{
			targeted_spot = player_spot+nbCell-1;
			tmp_spot = 6;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot-1]+dist[player_spot][7]) 
		{
			targeted_spot = player_spot-1; 
			tmp_spot = 7;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot-nbCell-1]+dist[player_spot][0]) 
		{
			targeted_spot = player_spot-nbCell-1; 
			tmp_spot = 0;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot-nbCell]+dist[player_spot][1]) 
			targeted_spot = player_spot-nbCell;
	}
	else if(y_b == nbCell-1)
	{
		targeted_spot = player_spot-1; tmp_spot = 7;
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot-nbCell-1]+dist[player_spot][0]) 
		{
			targeted_spot = player_spot-nbCell-1; 
			tmp_spot = 0;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot-nbCell]+dist[player_spot][1]) 
		{
			targeted_spot = player_spot-nbCell; 
			tmp_spot = 1;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot-nbCell+1]+dist[player_spot][2]) 
		{
			targeted_spot = player_spot-nbCell+1; 
			tmp_spot = 2;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot+1]+dist[player_spot][3]) 
			targeted_spot = player_spot+1;
	}
	else if(x_b == 0)
	{
		targeted_spot = player_spot-nbCell; tmp_spot = 1;
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot-nbCell+1]+dist[player_spot][2]) 
		{
			targeted_spot = player_spot-nbCell+1; 
			tmp_spot = 2;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot+1]+dist[player_spot][3]) 
		{
			targeted_spot = player_spot+1; 
			tmp_spot = 3;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot+nbCell+1]+dist[player_spot][4]) 
		{
			targeted_spot = player_spot+nbCell+1; 
			tmp_spot = 4;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot+nbCell]+dist[player_spot][5]) 
			targeted_spot = player_spot+nbCell;
	}
	else
	{
		targeted_spot = player_spot-nbCell-1; tmp_spot = 0;
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot-nbCell]+dist[player_spot][1]) 
		{
			targeted_spot = player_spot-nbCell; 
			tmp_spot = 1;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot-nbCell+1]+dist[player_spot][2]) 
		{
			targeted_spot = player_spot-nbCell+1; 
			tmp_spot = 2;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot+1]+dist[player_spot][3]) 
		{
			targeted_spot = player_spot+1; 
			tmp_spot = 3;
		};
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot+nbCell+1]+dist[player_spot][4]) 
		{
			targeted_spot = player_spot+nbCell+1; 
			tmp_spot = 4;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot+nbCell]+dist[player_spot][5]) 
		{
			targeted_spot = player_spot+nbCell; 
			tmp_spot = 5;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot+nbCell-1]+dist[player_spot][6]) 
		{
			targeted_spot = player_spot+nbCell-1; 
			tmp_spot = 6;
		}
		if(target[targeted_spot]+dist[player_spot][tmp_spot] > 
		target[player_spot-1]+dist[player_spot][7]) 
			targeted_spot = player_spot-1;
	}
	if(target[targeted_spot] == nbC_c) nogame = true;
	if(!nogame) 
	{
		double x(0), y(0);
		access_secret_player_db(x, y, i);
		fake_update(x, y, targeted_spot, i, nbCell);
		if(!collis_play_obs(nbCell, x, y))
		{
			update_indirect_position(targeted_spot, i, nbCell);
		}
		else
		{
			update_indirect_position(player_spot, i, nbCell);
		}
	}
}

void check_collisions_balles(int nbCell)
{
	vector<Player> players;
	copie_player_db(players);
	vector<Ball> balles;
	copie_ball_db(balles);
	vector<Carre> obs;
	copie_obstacle_db(obs);
	collis_ball_ball(nbCell);
	collis_joueur_ball(nbCell, players, balles);
	check_balle_adherance();
	collis_ball_obs(obs, balles, nbCell);
}

void reset_matrice()
{
	initialisee = false;
}

void end_game()
{
	increment_count();
	update_balls_position(get_static_nbCell());
	check_collisions_balles(get_static_nbCell());
}
