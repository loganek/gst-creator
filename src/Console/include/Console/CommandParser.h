/*
 * CommandParser.h
 *
 *  Created on: 29 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef COMMANDPARSER_H_
#define COMMANDPARSER_H_

#include <string>
#include "Commands/Command.h"

class CommandParser
{
private:
	std::string parsed_text;
	CommandType command_type;

	void parse_command();
	void syntax_error(const std::string& error);
public:
	CommandParser();
	virtual ~CommandParser();

	void parse(const std::string& text);
};

#endif /* COMMANDPARSER_H_ */
