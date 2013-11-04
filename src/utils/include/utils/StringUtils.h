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

class StringUtils
{
public:
	std::vector<std::string> split_string(std::string text, const std::string& delim);
};

#endif /* STRINGUTILS_H_ */
