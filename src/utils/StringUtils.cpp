/*
 * StringUtils.cpp
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#include "StringUtils.h"
#include <algorithm>
#include <sstream>
#include <iterator>

std::vector<std::string> StringUtils::split(std::string text,
		const std::string& delim)
{
	int pos;
	std::vector<std::string> values;

	while ((pos = text.find(delim)) != std::string::npos)
	{
		// TODO support arguments with " "
		std::string v = text.substr(0, pos);
		if (!v.empty())
			values.push_back(v);
		text.erase(0, pos + 1);
	}

	values.push_back(text);

	return values;
}

std::string StringUtils::join(const std::vector<std::string>& arr,
		const std::string& separator)
{
	std::ostringstream imploded;
	std::copy(arr.begin(), arr.end(),
			std::ostream_iterator<std::string>(imploded, separator.c_str()));

	return imploded.str();
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

bool StringUtils::is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
			s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

bool StringUtils::are_equal_case_no_sense(const std::string& s1, const std::string& s2)
{
	return to_upper(s1) == to_upper(s2);
}
