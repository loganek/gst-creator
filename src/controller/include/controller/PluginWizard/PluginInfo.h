/*
 * PluginInfo.h
 *
 * Plik zawiera definicję klasy PluginInfo
 *
 *  Created on: 13 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef PLUGININFO_H_
#define PLUGININFO_H_

#include <string>
#include <vector>

/**
 * Klasa przechowująca dane o wtyczce
 */
class PluginInfo
{
private:
	/** nazwa */
	std::string name;
	/** opis */
	std::string description;
	/** wersja */
	std::string version;
	/** licencja */
	std::string license;
	/** moduł źródłowy */
	std::string source_module;
	/** pakiet */
	std::string binary_package;
	/** pochodzenie */
	std::string origin_url;

public:
	/**
	 * Konstruktor
	 *
	 * @param name nazwa
	 * @param description opis
	 * @param version wersja
	 * @param license licencja
	 * @param source_module moduł źródłowy
	 * @param binary_package pakiet
	 * @param origin_url pochodzenie
	 */
	PluginInfo(std::string name, std::string description,
			std::string version, std::string license,
			std::string source_module, std::string binary_package,
			std::string origin_url);

	/**
	 * generuje kod źródłowy wtyczki
	 *
	 * @param names nazwy fabryk
	 * @param path lokalizacja wtyczki
	 */
	void generate_code(const std::vector<std::string>& names, const std::string& path);
};
#endif /* PLUGININFO_H_ */
