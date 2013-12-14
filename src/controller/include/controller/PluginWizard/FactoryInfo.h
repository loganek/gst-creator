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

	bool v_request_new_pad;
	bool v_release_pad;
	bool v_change_state;
	bool v_set_bus;
public:
	FactoryInfo(std::string name, std::string long_name,
			std::string klass, std::string author,
			std::string description, std::string parent, int rank);

	void set_virtual_methods(bool request_new_pad, bool release_pad, bool change_state, bool set_bus);
	void generate_header(const std::string& path);
	void generate_source(const std::string& path);

	std::string get_factory_name() const { return name; }
};

#endif /* FACTORYINFO_H_ */
