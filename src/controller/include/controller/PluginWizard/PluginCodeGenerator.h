/*
 * PluginCodeGenerator.h
 *
 * Plik zawiera definicję klasy PluginCodeGenerator
 *
 *  Created on: 13 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef PLUGINCODEGENERATOR_H_
#define PLUGINCODEGENERATOR_H_

#include "PluginInfo.h"
#include "FactoryInfo.h"
#include <string>
#include <vector>

/**
 * Klasa do generowania kodu źródłowego pluginów
 */
class PluginCodeGenerator
{
private:
	/** informacja na temat wtyczki */
	PluginInfo plugin;
	/** lista fabryk w ramach wtyczki */
	std::vector<FactoryInfo> factories;
public:
	/**
	 * konstruktor
	 *
	 * @param plugin informacja na temat wtyczki
	 * @param factories tablica fabryk w ramach wtyczki
	 */
	PluginCodeGenerator(const PluginInfo& plugin, const std::vector<FactoryInfo>& factories);

	/**
	 * generuje kod wtyczki
	 *
	 * @param path lokalizacja kodu źródłowego wtyczki
	 */
	void generate_code(const std::string& path);
};

#endif /* PLUGINCODEGENERATOR_H_ */
