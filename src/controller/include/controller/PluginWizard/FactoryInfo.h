/*
 * FactoryInfo.h
 *
 *  Created on: 13 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef FACTORYINFO_H_
#define FACTORYINFO_H_

#include <string>

class FactoryInfo
{
private:
	std::string name;
	std::string long_name;
	std::string klass;
	std::string author;
	std::string description;
	std::string parent;
	int rank;

public:
	FactoryInfo(std::string name, std::string long_name,
			std::string klass, std::string author,
			std::string description, std::string parent, int rank);

	void generate_header(const std::string& path);
	void generate_source(const std::string& path);

	std::string get_factory_name() const { return name; }
};

#endif /* FACTORYINFO_H_ */
