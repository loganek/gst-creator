/*
 * CommandParser.h
 *
 * Plik zawiera deklarację klasy CommandParser
 *
 *  Created on: 29 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef COMMANDPARSER_H_
#define COMMANDPARSER_H_

#include "Commands.h"
#include <string>
#include <gstreamermm.h>

/**
 * Klasa odpowiedzialna za parsowanie tekstu użytkownika wpisywanego z konsoli
 * do komendy
 */
class CommandParser
{
private:
	/**
	 * Komenda utworzona na podstawie tekstu użytkownika
	 */
	Command* command;
	/**
	 * Typ komendy
	 */
	CommandType type;
	/**
	 * Lista argumentów dla komendy
	 */
	std::vector<std::string> command_args;

	/**
	 * Model programu GStreamer
	 */
	Glib::RefPtr<Gst::Pipeline> model;

	/**
	 * Funkcja buduje obiekt typu Command
	 */
	void build_command();
public:
	/**
	 * Konstruktor, inicjalizuje pola klasy
	 *
	 * @param model model programu GStreamer
	 */
	CommandParser(const Glib::RefPtr<Gst::Pipeline>& model);

	/**
	 * Destruktor
	 */
	virtual ~CommandParser();

	/**
	 * Funkcja wykonująca parsowanie tekstu
	 *
	 * @param text tekst do sparsowania
	 *
	 * @returns  komenda wygenerowana na podstawie tekstu
	 */
	Command* parse(const std::string& text);
};

#endif /* COMMANDPARSER_H_ */
