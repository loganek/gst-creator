/*
 * CodeGenerator.h
 *
 * Plik zawiera definicję klasy CodeGenerator
 *
 *  Created on: 6 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef CODEGENERATOR_H_
#define CODEGENERATOR_H_

#include <gstreamermm.h>
#include <string>
#include <fstream>

/**
 * Klasa reprezentująca generator kodu
 */
class CodeGenerator
{
	/** Model programu GStreamer */
	Glib::RefPtr<Gst::Pipeline> model;

	/** strumień zapisu */
	std::ofstream output;

	/**
	 * Generuje cały kod źródłowy
	 */
	void generate_all();

	/**
	 * Generuje funkcję główną programu
	 */
	void generate_main();

	/**
	 * Generuje szablon klasy
	 */
	void generate_class();

	/**
	 * Generuje metodę get_pipeline
	 */
	void generate_get_pipeline();

	/**
	 * Generuje metodę create_pipeline
	 */
	void generate_create_pipeline();

	/**
	 * Generuje metodę init_elements
	 */
	void generate_init_elements();

	/**
	 * Generuje metodę static_links
	 */
	void generate_static_links();

	/**
	 * Generuje metodę dynamic_links
	 */
	void generate_dynamic_links();

	/**
	 * Generuje kod związany z ustawianiem właściwości
	 *
	 * @param element element, dla którego wygenerowany zostanie kod ustawiający właściwości
	 */
	void generate_properties(const Glib::RefPtr<Gst::Element>& element);
public:
	/**
	 * konstruktor
	 *
	 * @param model model programu GStreamer
	 */
	CodeGenerator(const Glib::RefPtr<Gst::Pipeline>& model);

	/**
	 * Generuje kod, i zapisuje go do pliku
	 *
	 * @param filename nazwa pliku, do którego zostanie zapisany kod
	 */
	void generate_code(const std::string& filename);
};

#endif /* CODEGENERATOR_H_ */
