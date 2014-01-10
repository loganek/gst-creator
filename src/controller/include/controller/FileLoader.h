/*
 * FileLoader.h
 *
 * Plik zawiera definicję klasy FileLoader
 *
 *  Created on: 4 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef FILELOADER_H_
#define FILELOADER_H_

#include <stack>
#include <gstreamermm.h>
#include <QtCore>
#include <functional>
#include "Commands.h"

/**
 * Klasa odpowiedzialna za ładowanie plików projektu do programu
 */
class FileLoader
{
public:
	/** typ funktora ustawiającego pozycję bloczka */
	typedef std::function<void(const Glib::RefPtr<Gst::Element>&, double, double)> position_setter;
private:
	/** obiekt odczytujący pliki XML */
	QXmlStreamReader reader;
	/** Nazwa pliku projektu */
	std::string filename;
	/** Model programu gstreamer */
	Glib::RefPtr<Gst::Pipeline> model;
	/** aktualnie ładowany element */
	Glib::RefPtr<Gst::Element> current_element;
	/** stos przetwarzanych elementów */
	std::stack<Glib::RefPtr<Gst::Element>> element_stack;
	/** mapa połączeń */
	std::map<Glib::ustring, Glib::ustring> connections;
	/** tablica obiektów nasłuchujących zmianę modelu */
	std::vector<CommandListener*> listeners;
	/** obiekt reprezentujący plik XML */
	QFile* file;
	/** obiekt funktora ustawiający pozycję bloczka */
	position_setter pos_setter;

	/**
	 * Pobiera atrybut z elementu XML
	 *
	 * @param attribute_name nazwa atrybutu
	 *
	 * @returns wartość atrybutu
	 */
	Glib::ustring get_attribute(const char* attribute_name);

	/**
	 * otwiera plik
	 */
	void open_file();

	/**
	 * Rozpoczyna przetwarzanie elementów
	 */
	void process_start_element();
public:
	/**
	 * Konstruktor
	 *
	 * @param filename nazwa pliku
	 * @param model model programu
	 * @param pos_setter funktor ustawiający pozycję bloczka
	 */
	FileLoader(const std::string& filename, const Glib::RefPtr<Gst::Pipeline>& model, position_setter pos_setter);

	/**
	 * Destruktor
	 */
	~FileLoader();

	/**
	 * Funkcja ładuje model z pliku projektu
	 *
	 * @param listeners obiekty nasłuchujące zmiany modelu
	 */
	void load_model(std::vector<CommandListener*> listeners);
};


#endif /* FILELOADER_H_ */
