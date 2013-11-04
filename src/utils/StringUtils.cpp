/*
 * StringUtils.cpp
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#include "StringUtils.h"

std::vector<std::string> StringUtils::split_string(std::string text, const std::string& delim)
{
	int pos;
	std::vector<std::string> values;

	while ((pos = text.find(" ")) != std::string::npos)
	{
		values.push_back(text.substr(0, pos));
		text.erase(0, pos + 1);
	}

	values.push_back(text);

	return values;
}
