/*
 * CommandParser.cpp
 *
 *  Created on: 29 pa? 2013
 *      Author: Marcin Kolny
 */

#include "CommandParser.h"
#include <stdexcept>

CommandParser::CommandParser()
{

}

CommandParser::~CommandParser()
{

}

/**
 * Command list:
 *  - add
 *    - element [to bin1:bin2:bin3 default=pipeline] factory
 *    - pad [bin1:bin2:element default=pipeline:element] using template
 *    - connection between
 *      - elements element1 element2
 *      - pads element1:pad1 element2:pad2
 *
 *  - remove
 *    - element
 *    - pad
 *    - connection between
 *      - elements
 *      - pads
 *  - reconnect
 *    - {pad | element}
 *    - to {pad | element}
 *
 */
#include <iostream>
void CommandParser::parse(const std::string& text)
{
	parsed_text = text;

	int end = parsed_text.find(' ');

	if (end == std::string::npos)
		end = parsed_text.length();

	std::string command = parsed_text.substr(0, end);

	if (command == "add")
	{
		std::cout << "add command" << std::endl;
	}
	else if (command == "remove")
	{
		std::cout << "remove command" << std::endl;
	}
	else if (command == "reconnect")
	{
		std::cout << "reconnect command" << std::endl;
	}
	else
	{
		syntax_error("unknow command `" + command + "`");
	}
}

void CommandParser::syntax_error(const std::string& error)
{
	throw std::runtime_error("Syntax error: " + error);
}
