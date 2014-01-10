/*
 * PropertyCommand.h
 *
 * Plik zawiera deklarację klasy PropertyCommand
 *
 *  Created on: 9 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef PROPERTYCOMMAND_H_
#define PROPERTYCOMMAND_H_



#include "Command.h"
#include "Properties/Property.h"
#include <vector>
#include <gstreamermm.h>

/**
 * Klasa reprezentująca komendę zmieniającą właściwość elementu
 */
class PropertyCommand : public Command
{
private:
	/** Obiekt reprezentujący zmienianą właściwość */
	Property* property;
	/** Określa, czy ma zostać wywołane okienko z listą wszystkich właściwości */
	bool run_window;
	/** Element, z którego pobierane są właściwości */
	Glib::RefPtr<Gst::Element> element;

public:
	/**
	 * Konstruktor, inicjalizuje pola klasy
	 *
	 * @param element obiekt na którym wykonywana będzie operacja zmiany właściwości
	 * @param property_name nazwa zmienianej właściwości
	 * @param property_value nowa wartość właściwości
	 */
	PropertyCommand(
			const Glib::RefPtr<Gst::Element>& element,
			const std::string& property_name,
			const std::string& property_value);

	/**
	 * Destruktor
	 */
	virtual ~PropertyCommand();

	/**
	 * Tworzy obiekt PropertyCommand na podstawie listy argumentów
	 *
	 * @param args lista argumentów
	 * @param model model programu GStreamer
	 */
	static PropertyCommand* from_args(const std::vector<std::string>& vect, const Glib::RefPtr<Gst::Pipeline>& model);

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


#endif /* PROPERTYCOMMAND_H_ */
