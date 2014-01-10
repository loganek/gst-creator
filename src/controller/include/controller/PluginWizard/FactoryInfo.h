/*
 * FactoryInfo.h
 *
 * Plik zawiera definicję klasy FactoryInfo
 *
 *  Created on: 13 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef FACTORYINFO_H_
#define FACTORYINFO_H_

#include <string>

/**
 * Klasa przedstawiająca informację o fabryce dla generatora wtyczek
 */
class FactoryInfo
{
private:
	/** nazwa fabryki */
	std::string name;
	/** "długa nazwa fabryki */
	std::string long_name;
	/** klasa */
	std::string klass;
	/** autor */
	std::string author;
	/** opis */
	std::string description;
	/** rodzic */
	std::string parent;
	/** ranking, ważność fabryki */
	int rank;

	/** określa, czy generować metodę wirtualną request_new_pad */
	bool v_request_new_pad;
	/** określa, czy generować metodę wirtualną release_pad */
	bool v_release_pad;
	/** określa, czy generować metodę wirtualną change_state */
	bool v_change_state;
	/** określa, czy generować metodę wirtualną set_bus */
	bool v_set_bus;
public:
	/**
	 * Konstruktor
	 *
	 * @param name nazwa fabryki
	 * @param long_name "długa nazwa" fabryki
	 * @param klasa
	 * @param autor
	 * @param opis
	 * @param rodzic
	 * @param ranking, ważność fabryki
	 */
	FactoryInfo(std::string name, std::string long_name,
			std::string klass, std::string author,
			std::string description, std::string parent, int rank);

	/**
	 * Określa, które wirtualne metody mają być generowane
	 *
	 * @param request_new pad generuj funkcję request_new_pad
	 * @param release_pad pad generuj funkcję release_pad
	 * @param change_state pad generuj funkcję change_state
	 * @param set_bus pad generuj funkcję set_bus
	 */
	void set_virtual_methods(bool request_new_pad, bool release_pad, bool change_state, bool set_bus);

	/**
	 * Generuje plik nagłówka
	 *
	 * @param path ścieżka pliku
	 */
	void generate_header(const std::string& path);

	/**
	 * Generuje plik źródłowy
	 *
	 * @param path ścieżka pliku
	 */
	void generate_source(const std::string& path);

	/**
	 * zwraca nazwę fabryki
	 *
	 * @returns nazwa fabryki
	 */
	std::string get_factory_name() const { return name; }
};

#endif /* FACTORYINFO_H_ */
