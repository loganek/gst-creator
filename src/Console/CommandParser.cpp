/*
 * CommandParser.cpp
 *
 *  Created on: 29 pa? 2013
 *      Author: Marcin Kolny
 */

#include "CommandParser.h"
#include "Commands/enum_string_converter.h"
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
 *
 *  - remove
 *    - element
 *    - pad
 *
 *  - connect
 *    - elements element1 to element2
 *    - pads element1:pad1 element2:pad2
 *  - reconnect
 *    - {pad | element}
 *    - to {pad | element}
 *
 */
#include <iostream>
void CommandParser::parse(const std::string& text)
{
	parsed_text = text;

	parse_command();
}

void CommandParser::parse_command()
{
	int end = parsed_text.find(' ');

	if (end == std::string::npos)
		end = parsed_text.length();

	std::string command = parsed_text.substr(0, end);

	try
	{
		command_type = string_to_enum<CommandType>(command);
	}
	catch (std::runtime_error&)
	{
		syntax_error("unknown command: " + command);
	}
}

void CommandParser::syntax_error(const std::string& error)
{
	throw std::runtime_error("Syntax error: " + error);
}
