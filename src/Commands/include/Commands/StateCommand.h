/*
 * StateCommand.h
 *
 * Plik zawiera deklarację klasy StateCommand
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef STATECOMMAND_H_
#define STATECOMMAND_H_

#include "Command.h"
#include <gstreamermm.h>

/**
 * Klasa reprezentująca komendę zmieniającą stan modelu programu GStreamer
 */
class StateCommand : public Command
{
	/** nowy stan modelu programu */
	State state;
	/** model programu */
	Glib::RefPtr<Gst::Pipeline> model;
public:
	/**
	 * Konstruktor, inicjalizuje pola klasy
	 *
	 * @param state nowy stan modelu programu, jaki ma ustawić komenda
	 * @param model model programu GStreamer
	 */
	StateCommand(State state, const Glib::RefPtr<Gst::Pipeline>& model);

	/**
	 * Tworzy obiekt StateCommand na podstawie listy argumentów
	 *
	 * @param args lista argumentów
	 * @param model model programu GStreamer
	 */
	static StateCommand* from_args(const std::vector<std::string>& vect, const Glib::RefPtr<Gst::Pipeline>& model);

	/**
	 * Zwraca listę podpowiedzi następnego słowa dla zadanej listy argumentów
	 *
	 * @param args lista argumentów
	 * @param model model programu GStreamer
	 *
	 * @returns lista podpowiedzi
	 */
	static std::vector<std::string> get_suggestions(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);

	/**
	 * Uruchamia komendę
	 *
	 * @param listeners tablica obiektów nasłuchujących zmiany wprowadzone po wykonaniu komendy
	 */
	void run_command(std::vector<CommandListener*> listeners = {});
};

#endif /* STATECOMMAND_H_ */
