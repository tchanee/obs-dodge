#include <iostream>
#include <fstream>
#include <cairomm/context.h>
#include <gtkmm/filechooserdialog.h>
#include <glibmm.h>
#include "gui.h"
#include "define.h"
#include "simulation.h"
#define R_BACKGROUND 1.0
#define G_BACKGROUND 1.0
#define B_BACKGROUND 1.0
#define R_FRAME 0.0
#define G_FRAME 0.0
#define B_FRAME 0.0
#define R_GREEN 0.27
#define G_GREEN 0.54
#define B_GREEN 0.0
#define R_YELLOW 1.0
#define G_YELLOW 0.84
#define B_YELLOW 0.0
#define R_ORANGE 1.0
#define G_ORANGE 0.64
#define B_ORANGE 0.0
#define R_RED 0.8
#define G_RED 0.0
#define B_RED 0.0
#define R_SOURCE_OBSTACLE 0.5
#define G_SOURCE_OBSTACLE 0.0
#define B_SOURCE_OBSTACLE 0.0
#define R_SOURCE_ATTACK 0.203
#define G_SOURCE_ATTACK 0.601
#define B_SOURCE_ATTACK 1.0
#define ANGLE_DEBUT_CERCLE 0
#define ANGLE_FIN_CERCLE 7 //ce choix assure simmplement que le 
						   //cercle soit complet; C'est arbitraire

using namespace std;

enum HEALTH { RED = 1, ORANGE, YELLOW, GREEN };

static bool endgame(false);
static bool nogame(false);

Espace_dessin::Espace_dessin() : empty(false)
{
}

Espace_dessin::~Espace_dessin()
{
}

void Espace_dessin::dessiner()
{
	empty = false;
	refresh();
}

void Espace_dessin::effacer()
{
	clear_nbCell();
	clear_player_secret_db();
	clear_obstacle_secret_db();
	clear_ball_secret_db();
	empty = true;
	refresh();
}

void Espace_dessin::refresh()
{
	auto win = get_window();
	if(win)
	{
		Gdk::Rectangle r(0, 0, get_allocation().get_width(),
							   get_allocation().get_height());
		win->invalidate_rect(r, false);
	}
}

void dessin_players(const Cairo::RefPtr<Cairo::Context>& cr)
{
	size_t nb_players(0);
	get_nb_players(nb_players);
	double x(0), y(0), rayon(0), count(0);
	int nb_touch(0);
	
	for(size_t i(0); i < nb_players; ++i)
	{	
		get_player_specifications(x, y, rayon, nb_touch, count, i);
		cr->set_line_width(rayon);
		if(nb_touch == (int)GREEN)
		{ 
			cr->set_source_rgb(R_GREEN, G_GREEN, B_GREEN);
		}
		else if(nb_touch == (int)YELLOW)
		{ 
			cr->set_source_rgb(R_YELLOW, G_YELLOW, B_YELLOW); 
		}
		else if(nb_touch == (int)ORANGE)
		{
			 cr->set_source_rgb(R_ORANGE, G_ORANGE, B_ORANGE); 
		}
		else if(nb_touch == (int)RED)
		{
			cr->set_source_rgb(R_RED, G_RED, B_RED);
		}
		cr->arc(x + DIM_MAX, DIM_MAX - y, rayon/2, ANGLE_DEBUT_CERCLE, 
				ANGLE_FIN_CERCLE);
		cr->stroke();
		
		double loading_attack(((count)/MAX_COUNT)*2*M_PI);
		
		cr->set_line_width(rayon/2.5);
		cr->set_source_rgb(R_SOURCE_ATTACK, G_SOURCE_ATTACK, 
						   B_SOURCE_ATTACK);
		cr->arc(x + DIM_MAX, DIM_MAX - y, rayon/1.25, -M_PI/2, 
				-M_PI/2+loading_attack);
		cr->stroke();
	}	
}

void dessin_obstacles(const Cairo::RefPtr<Cairo::Context>& cr)
{
	size_t nb_obstacles(0);
	get_nb_obstacles(nb_obstacles);
	cr->set_source_rgb(R_SOURCE_OBSTACLE, G_SOURCE_OBSTACLE, 
					   B_SOURCE_OBSTACLE);
	int lgn(0), col(0);
	
	for(size_t i(0); i < nb_obstacles; ++i)
	{
		
		get_obstacle_specifications(lgn, col, i);
		cr->set_line_width(SIDE/copie_nbCell());
		cr->move_to((col + 0.5)*(SIDE/copie_nbCell()),
				   ((((lgn) + 1)*(SIDE/copie_nbCell())))); 
		cr->line_to((col + 0.5)*(SIDE/copie_nbCell()),
				   (((lgn)*(SIDE/copie_nbCell()))));
		cr->stroke();
	}
}

void dessin_balls(const Cairo::RefPtr<Cairo::Context>& cr)
{
	size_t nb_balls(0);
	get_nb_balls(nb_balls);
	cr->set_source_rgb(R_SOURCE_ATTACK, G_SOURCE_ATTACK, 
					   B_SOURCE_ATTACK);
	double x(0), y(0), rayon(0);
	
	for(size_t i(0); i < nb_balls; ++i)
	{
		get_ball_specifications(x, y, rayon, i);
		cr->set_line_width(rayon);
		cr->arc(x + DIM_MAX, DIM_MAX - y, rayon/2, ANGLE_DEBUT_CERCLE, 
				ANGLE_FIN_CERCLE);
		cr->stroke();
	}
}

bool Espace_dessin::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{	
	cr->set_source_rgb(R_BACKGROUND, G_BACKGROUND, B_BACKGROUND);
	cr->paint();
	cr->set_source_rgb(R_FRAME, G_FRAME, B_FRAME);
	cr->set_line_width(2);
	cr->rectangle(0, 0, SIDE, SIDE);
	cr->stroke();
	
	dessin_players(cr);
	dessin_obstacles(cr);
	dessin_balls(cr);
	
	return true;
}

Fenetre::Fenetre()
:   m_box(Gtk::ORIENTATION_VERTICAL, 10),
	m_box_top(Gtk::ORIENTATION_HORIZONTAL, 0),
	m_box_bottom(Gtk::ORIENTATION_VERTICAL,0),
	m_button_exit     ("_Exit", true ),
	m_button_open     ("_Open", true ),
	m_button_save     ("_Save", true ),
	m_button_start	  ("_Start", true),
	m_button_step     ("_Step", true ),
	m_label_game_state("_No game to run", true),
	timeout_value(DELTA_T*1000),
	game_started(false),
	stop_game(false)
{
	set_title("Dodgeball");
	set_border_width(0);
	
	add(m_box);
	m_box.pack_start(m_box_top);
	m_box.pack_start(m_box_bottom);
	m_dessin.set_size_request(SIDE, SIDE);
	m_box_bottom.pack_start(m_dessin) ;
	m_box_top.pack_start(m_button_exit,      false, false);
	m_box_top.pack_start(m_button_open,      false, false);
	m_box_top.pack_start(m_button_save,      false, false);
	m_box_top.pack_start(m_button_start,     false, false);
	m_box_top.pack_start(m_button_step,      false, false);
	m_box_top.pack_start(m_label_game_state, false, false);
	
	m_button_exit.signal_clicked().connect(sigc::mem_fun(*this ,
				&Fenetre::on_button_clicked_exit ) );
	m_button_open.signal_clicked().connect(sigc::mem_fun(*this ,
				&Fenetre::on_button_clicked_open ) );
	m_button_save.signal_clicked().connect(sigc::mem_fun(*this ,
				&Fenetre::on_button_clicked_save ) );
	m_button_start.signal_clicked().connect(sigc::mem_fun(*this,
				&Fenetre::on_button_clicked_start) );
	m_button_step.signal_clicked().connect(sigc::mem_fun(*this ,
				&Fenetre::on_button_clicked_step ) );
							
	show_all_children();			
}

Fenetre::~Fenetre()
{
}

void Fenetre::dessiner()
{
	m_dessin.dessiner();
}

void Fenetre::effacer()
{
	m_dessin.effacer();
}

void Fenetre::on_button_clicked_exit()
{
	exit(EXIT_SUCCESS);
}

void Fenetre::on_button_clicked_open()
{
	Gtk::FileChooserDialog dialog("Please choose a file", 
			Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);
	
	int result = dialog.run();
	string file = dialog.get_filename();
	if(result == Gtk::RESPONSE_OK)
	{
		bool a_ouvrir(true);
		bool arret(false);
		m_dessin.effacer();
		lecture_error_check(file, a_ouvrir, arret);
		if(!arret) m_dessin.dessiner();
		else  if(arret)
		{
			m_dessin.effacer();
		}
		if(!arret) m_label_game_state.set_text("Game ready to run");
		if(arret)  m_label_game_state.set_text("No game to run");
		reset_matrice();
		endgame = false;
		nogame = false;
	}
}

void Fenetre::on_button_clicked_save()
{
	Gtk::FileChooserDialog dialog("Please choose a file", 
			Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for(*this);
	
	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Save", Gtk::RESPONSE_OK);
	
	int result = dialog.run();
	if(result == Gtk::RESPONSE_OK)
	{	
		string file = dialog.get_filename();
		save(file);
		m_label_game_state.set_text("Game ready to run");
	}
}

void Fenetre::on_button_clicked_start()
{
	if(!game_started)
	{
		Glib::signal_timeout().connect(sigc::mem_fun(*this, 
							 &Fenetre::on_timeout), timeout_value);
		game_started = true;
		m_button_start.set_label("_Stop");
	}
	else
	{
		m_button_start.set_label("_Start");
		game_started = false;
		stop_game = true;	
	}
}

void Fenetre::on_button_clicked_step()
{
	if(nogame)
	{
		m_label_game_state.set_text("Cannot complete game!");
	}
	else if(endgame)
	{
		end_game();
		m_label_game_state.set_text("Game Over!");
	}
	else
	{
		simulate(endgame, nogame);
		m_dessin.dessiner();
	}
}

void Fenetre::set_label_game_state_ready()
{
	m_label_game_state.set_text("Game ready to run");
}

void Fenetre::set_label_game_state_no_file()
{
	m_label_game_state.set_text("No game to run");
}

bool Fenetre::on_timeout()
{
	if(stop_game)
	{
		stop_game = false;
		return false;
	}
	if(nogame)
	{
		end_game();
		m_label_game_state.set_text("Cannot complete game!");
		m_dessin.dessiner();
		return true;
	}
	else if(endgame)
	
	{
		end_game();
		m_label_game_state.set_text("Game Over!");
		m_dessin.dessiner();
		return true;
	}
	else
	{
		simulate(endgame, nogame);
		m_dessin.dessiner();
		return true;
	}
}
