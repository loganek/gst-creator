/*
 * StringUtils.cpp
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#include "StringUtils.h"
#include <algorithm>

std::vector<std::string> StringUtils::split(std::string text, const std::string& delim)
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

std::string StringUtils::trim(std::string text)
{
	int pos = text.find_last_not_of(' ');
	if(pos != std::string::npos)
	{
		text.erase(pos + 1);
		pos = text.find_first_not_of(' ');
		if(pos != std::string::npos) text.erase(0, pos);
	}
	else text.erase(text.begin(), text.end());

	return text;
}

std::string StringUtils::to_upper(std::string text)
{
	std::transform(text.begin(), text.end(), text.begin(), ::toupper);
	return text;
}
