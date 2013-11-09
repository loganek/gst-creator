/*
 * StringUtils.h
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

#include <vector>
#include <string>
#include <sstream>

class StringUtils
{
public:
	static std::vector<std::string> split(std::string text, const std::string& delim);
	static std::string trim(std::string text);
	static std::string to_upper(std::string text);

	template<typename T>
	static T str_to_numeric(const std::string& value)
	{
		std::stringstream ss;
		T val;
		ss << value;
		ss >> val;
		return val;
	}
};

#endif /* STRINGUTILS_H_ */
