/*
 * FileWriter.h
 *
 * Plik zawiera definicję klasy FileWriter
 *
 *  Created on: 4 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef FILEWRITER_H_
#define FILEWRITER_H_

#include <gstreamermm.h>
#include <QtCore>
#include <functional>

/**
 * Klasa odpowiedzialna za zapis pliku projektu
 */
class FileWriter
{
public:
	/** typ funktora znajdującego lokalizację elementu */
	typedef std::function<QPointF(const Glib::RefPtr<Gst::Element>&)> find_block;
private:
	/** model programu */
	Glib::RefPtr<Gst::Pipeline> model;
	/** nazwa pliku */
	std::string filename;
	/** obiekt do zapisu pliku XML */
	QXmlStreamWriter writer;
	/** reprezentacja pliku */
	QFile* file;
	/** obiekt funktora znajdującego lokalizację elementu */
	find_block finder;

	/**
	 * Zapisuje pojedynczy element
	 *
	 * @param element element zapisu
	 */
	void write_single_element(const Glib::RefPtr<Gst::Element>& element);

	/**
	 * Zapisuje połączenia przyszłościowe
	 */
	void write_future_connections();

	/**
	 * Otwiera plik do zapisu
	 */
	void open_file();

public:
	/**
	 * konstruktor
	 *
	 * @param filename nazwa pliku
	 * @param model model programu
	 * @param obiekt funktora odszukującego lokalizację bloczka elementu
	 */
	FileWriter(const std::string& filename, const Glib::RefPtr<Gst::Pipeline>& model, find_block finder);

	/**
	 * Destruktor
	 */
	virtual ~FileWriter();

	/**
	 * Zapisuje model do pliku projektu
	 */
	void save_model();
};

#endif /* FILEWRITER_H_ */
