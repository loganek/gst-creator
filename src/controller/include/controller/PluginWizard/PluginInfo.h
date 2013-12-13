/*
 * PluginInfo.h
 *
 *  Created on: 13 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef PLUGININFO_H_
#define PLUGININFO_H_

#include <string>
#include <vector>

class PluginInfo
{
private:
	std::string name;
	std::string description;
	std::string version;
	std::string license;
	std::string source_module;
	std::string binary_package;
	std::string origin_url;

public:
	PluginInfo(std::string name, std::string description,
			std::string version, std::string license,
			std::string source_module, std::string binary_package,
			std::string origin_url);

	void generate_code(const std::vector<std::string>& names, const std::string& path);
};
#endif /* PLUGININFO_H_ */
