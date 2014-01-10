/*
 * AddCommand.h
 *
 * Plik zawiera deklarację klasy AddCommand
 *
 *  Created on: 2 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef ADDCOMMAND_H_
#define ADDCOMMAND_H_

#include "Command.h"
#include "CommandListener.h"
#include <vector>
#include <gstreamermm.h>

/**
 * Klasa reprezentująca komendę dodającą obiekt do modelu programu
 */
class AddCommand : public Command
{
private:
	/**
	 * Obiekt, jaki zostanie dodany podczas wykonania komendy
	 */
	Glib::RefPtr<Gst::Object> object;
	/**
	 * Rodzic dla dodawanego obiektu
	 */
	Glib::RefPtr<Gst::Element> parent;
	/**
	 * Typ dodawanego elementu
	 */
	ObjectType type;

	static AddCommand* generate_add_pad_command(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);
	static AddCommand* generate_add_element_command(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);
public:
	/**
	  * Konstruktor, inicjalizuje pola klasy.
	  *
	  * @param type typ obiektu, jaki zostanie dodany
	  * @param parent obiekt nadrzędny, do którego zostanie dodany obiekt
	  * @param object obiekt, który zostanie dodany do obiektu nadrzędnego
	 */
	AddCommand(ObjectType type,
			const Glib::RefPtr<Gst::Element>& parent,
			const Glib::RefPtr<Gst::Object>& obj);

	/**
	 * Destruktor klasy
	 */
	virtual ~AddCommand();

	/**
	 * Tworzy obiekt AddCommand na podstawie listy argumentów
	 *
	 * @param args lista argumentów
	 * @param model model programu GStreamer
	 */
	static AddCommand* from_args(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);

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

	/**
	 * Uruchamia komendę, zwracając dodany obiekt
	 *
	 * @param listeners tablica obiektów nasłuchujących zmiany wprowadzone po wykonaniu komendy
	 *
	 * @returns dodany obiekt
	 */
	Glib::RefPtr<Gst::Object> run_command_ret(std::vector<CommandListener*> listeners = {});

	/**
	 * Zwraca obiekt, który zostanie dodany
	 *
	 * @returns obiekt, który zostanie dodany
	 */
	Glib::RefPtr<Gst::Object> get_object() { return object; }
};

#endif /* ADDCOMMAND_H_ */
