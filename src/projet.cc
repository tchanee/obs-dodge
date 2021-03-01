#include <iostream>
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include "simulation.h"
#include "gui.h"
#include "define.h"

#define PREMIER_ARGUMENT 1
#define DEUXIEME_ARGUMENT 2

using namespace std;

void ouvrir_et_save(char * file_a_ouvrir, char * file_a_save);

int main(int argc , char * argv[])
{	
	int filler_arg(1);
	if(argc == 3) 
	{
		bool a_ouvrir(false);
		bool arret(false);
		lecture_error_check(argv[DEUXIEME_ARGUMENT], a_ouvrir, arret);
	}
	else if(argc == 1)
	{
		auto app=Gtk::Application::create(filler_arg, argv,
										  "org.gtkmm.exemple");								
		Fenetre win;
		win.set_size_request(SIDE, SIDE);
		win.set_resizable(false);
		return app->run(win);
	}
	else if(argc == 2)
	{
		auto app=Gtk::Application::create(filler_arg, argv,
										  "org.gtkmm.exemple");
		bool a_ouvrir(true);
		bool arret(false);
		lecture_error_check(argv[PREMIER_ARGUMENT], a_ouvrir, arret);									
		Fenetre win;
		if(arret) 
		{
			win.effacer();
			win.set_label_game_state_no_file();
		}
		win.set_size_request(SIDE, SIDE);
		win.set_resizable(false);
		if(!arret) win.set_label_game_state_ready();
		return app->run(win);
	}
	else if(argc == 4)
	{
		ouvrir_et_save(argv[2], argv[3]);
	}
	return EXIT_SUCCESS;
}

void ouvrir_et_save(char * file_a_ouvrir, char * file_a_save)
{
	bool a_ouvrir(true);
	bool arret(false);
	string file_save_string(file_a_save);
	lecture_error_check(file_a_ouvrir, a_ouvrir, arret);
	simulate(arret, arret);
	save(file_save_string);
}
