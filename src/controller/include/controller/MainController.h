/*
 * MainController.h
 *
 * Plik zawiera definicję klasy MainController
 *
 *  Created on: 4 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef MAINCONTROLLER_H_
#define MAINCONTROLLER_H_

#include "Commands/CommandListener.h"
#include <gstreamermm.h>
#include <string>

class MainWindow;

/**
 * Klasa głównego kontrolera
 */
class MainController : public CommandListener
{
private:
	/** model programu */
	Glib::RefPtr<Gst::Pipeline> model;
	/** nazwa aktualnie edytowanego pliku projektu */
	std::string current_project_file;
	/** czy model programu został zmodyfikowany od ostatniego zapisu? */
	bool model_modified_state;
	/** okno główne */
	MainWindow* main_view;

	/** ustawia stan modyfikacji projektu na "zmodyfikowany" */
	void set_modified_state();

public:
	/**
	 * Konstruktor
	 *
	 * @param model model programu
	 */
	MainController(const Glib::RefPtr<Gst::Pipeline>& model);

	/**
	 * Destruktor
	 */
	virtual ~MainController();

	/**
	 * Zwraca model programu
	 *
	 * @returns model programu
	 */
	Glib::RefPtr<Gst::Pipeline> get_model() const;

	/**
	 * Ustawia widok
	 *
	 * @param main_view okno główne programu
	 */
	void set_main_view(MainWindow* main_view);

	/**
	 * Ustawia nazwę pliku projektu
	 *
	 * @param current_project_file nazwa pliku projektu
	 */
	void set_current_project_file(const std::string& current_project_file);

	/**
	 * Zwraca nazwę pliku projektu
	 *
	 * @returns nazwa pliku projektu
	 */
	std::string get_current_project_file() const;

	/**
	 * resetuje stan zmodyfikowania projektu
	*/
	void reset_modified_state();

	/**
	 * sprawdza stan zmodyfikowania obiektu
	 *
	 * @returns true, jeśli plik został zmodyfikowany od ostatniego zapisu
	 */
	bool get_modified_state() const;

	/***** Implementacja CommandListener *****/
	void pad_added(const Glib::RefPtr<Gst::Pad>& pad)
	{set_modified_state();}
	void pad_removed(const Glib::RefPtr<Gst::Pad>& pad)
	{set_modified_state();}
	void pad_linked(const Glib::RefPtr<Gst::Pad>& pad)
	{set_modified_state();}
	void pad_unlinked(const Glib::RefPtr<Gst::Pad>& pad)
	{set_modified_state();}
	void future_connection_added(const Glib::RefPtr<Gst::PadTemplate>& src_tpl,
			const Glib::RefPtr<Gst::Element>& parent, const Glib::RefPtr<Gst::Pad>& sink)
	{set_modified_state();}
	void future_connection_removed(const ConnectCommand::future_connection_pads& conn)
	{set_modified_state();}

	/**
	 * Czyści model programu
	 */
	void clean_model();
};

#endif /* MAINCONTROLLER_H_ */
