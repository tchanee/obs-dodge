//Interface du module gui

#ifndef GUI_H
#define GUI_H

#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>
#include <gtkmm/application.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>

class Espace_dessin : public Gtk::DrawingArea
{
public :
	Espace_dessin();
	virtual ~Espace_dessin();
	void dessiner();
	void effacer();

protected:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
	
private:
	bool empty;
	void refresh();
};

class Fenetre : public Gtk::Window
{
public:
	Fenetre();
	virtual ~Fenetre();
	void dessiner();
	void effacer();
	void set_label_game_state_ready();
	void set_label_game_state_no_file();
	
protected:
	void on_button_clicked_exit ();
	void on_button_clicked_open ();
	void on_button_clicked_save ();
	void on_button_clicked_start();
	void on_button_clicked_step ();
	bool on_timeout();

	Gtk::Box m_box, m_box_top, m_box_bottom; 
	Espace_dessin m_dessin;
	Gtk::Button m_button_exit       ;
	Gtk::Button m_button_open 	 	;
	Gtk::Button m_button_save  	 	;
	Gtk::Button m_button_start		;
	Gtk::Button m_button_step 		;
	Gtk::Label  m_label_game_state  ;
	const int timeout_value;
	bool game_started;
	bool stop_game;
};

#endif
