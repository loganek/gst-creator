/*
 * PluginCodeGenerator.h
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

class PluginCodeGenerator
{
private:
	PluginInfo plugin;
	std::vector<FactoryInfo> factories;
public:
	PluginCodeGenerator(const PluginInfo& plugin, const std::vector<FactoryInfo>& factories);
	void generate_code(const std::string& path);
};

#endif /* PLUGINCODEGENERATOR_H_ */
