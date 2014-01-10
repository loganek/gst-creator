/*
 * RemoveCommand.h
 *
 * Plik zawiera deklarację klasy RemoveCommand
 *
 *  Created on: 16 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef REMOVECOMMAND_H_
#define REMOVECOMMAND_H_

#include "Command.h"
#include <vector>
#include <gstreamermm.h>

/**
 * Klasa reprezentująca komendę usuwającą obiekt z modelu programu
 */
class RemoveCommand : public Command
{
private:
	/** Obiekt, jaki zostanie usunięty */
	Glib::RefPtr<Gst::Object> object;
	/** Rodzic usuwanego obiektu */
	Glib::RefPtr<Gst::Element> parent;
	/** Typ usuwanego elementu */
	ObjectType type;

public:
	/**
	 * Konstruktor, inicjalizuje pola klasy.
	 *
	 * @param type typ usuwanego obiektu
	 * @param obj obiekt do usunięcia
	 */
	RemoveCommand(ObjectType type,
			const Glib::RefPtr<Gst::Object>& obj);

	/**
	 * Destruktor
	 */
	virtual ~RemoveCommand();

	/**
	 * Tworzy obiekt RemoveCommand na podstawie listy argumentów
	 *
	 * @param args lista argumentów
	 * @param model model programu GStreamer
	 */
	static RemoveCommand* from_args(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);

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

#endif /* REMOVECOMMAND_H_ */
