/*
 * PluginCodeGenerator.cpp
 *
 *  Created on: 13 gru 2013
 *      Author: Marcin Kolny
 */

#include "PluginWizard/PluginCodeGenerator.h"
#include "utils/StringUtils.h"

using namespace std;

PluginCodeGenerator::PluginCodeGenerator(const PluginInfo& plugin, const vector<FactoryInfo>& factories)
: plugin(plugin),
  factories(factories)
{

}

void PluginCodeGenerator::generate_code(const string& path)
{
	vector<string> names;

	for (auto factory : factories)
	{
		factory.generate_header(path);
		factory.generate_source(path);
		names.push_back(factory.get_factory_name());
	}

	plugin.generate_code(names, path);
}


