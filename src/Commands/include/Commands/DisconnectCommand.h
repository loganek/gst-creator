/*
 * DisconnectCommand.h
 *
 * Plik zawiera deklarację klasy DisconnectCommand
 *
 *  Created on: 16 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef DISCONNECTCOMMAND_H_
#define DISCONNECTCOMMAND_H_

#include "ConnectCommand.h"
#include <gstreamermm.h>


/**
 * Klasa reprezentująca komendę usuwającą połączenie pomiędzy obiektami
 */
class DisconnectCommand : public Command
{
private:
	/** Obiekt źródłowy połączenia */
	Glib::RefPtr<Gst::Object> src;
	/** Obiekt docelowy połączenia */
	Glib::RefPtr<Gst::Object> dst;
	/** Typ obiektów połączenia */
	ObjectType type;
public:
	/**
	 * Destruktor, inicjalizuje pola klasy
	 *
	 * @param src obiekt źródłowy połączenia
	 * @param dst obiekt docelowy połączenia
	 */
	DisconnectCommand(const Glib::RefPtr<Gst::Object>& src, const Glib::RefPtr<Gst::Object>& dst);

	/**
	 * Tworzy obiekt DisconnectCommand na podstawie listy argumentów
	 *
	 * @param args lista argumentów
	 * @param model model programu GStreamer
	 */
	static DisconnectCommand* from_args(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);

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

#endif /* DISCONNECTCOMMAND_H_ */
