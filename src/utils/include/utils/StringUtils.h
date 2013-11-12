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
	static std::vector<std::string> split(std::string text,
			const std::string& delim);
	static std::string join(const std::vector<std::string>& arr,
			const std::string& separator);
	static std::string trim(std::string text);
	static std::string to_upper(std::string text);
	static bool is_number(const std::string& s);
	static bool are_equal_case_no_sense(const std::string& s1, const std::string& s2);

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
