/*
 * CommandParser.cpp
 *
 *  Created on: 29 pa? 2013
 *      Author: Marcin Kolny
 */

#include "CommandParser.h"
#include "Commands/enum_string_converter.h"
#include <stdexcept>

CommandParser::CommandParser(const Glib::RefPtr<Gst::Pipeline>& model)
: model(model)
{

}

CommandParser::~CommandParser()
{

}

/**
 * Command list:
 *  - add
 *    - element factory [to bin1:bin2:bin3 default=pipeline]
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
Command* CommandParser::parse(const std::string& text)
{
	split_command_text(text);

	try
	{
		type = string_to_enum<CommandType>(command_args[0]);
	}
	catch (std::runtime_error&)
	{
		Command::syntax_error("unknown command: " + command_args[0]);
	}

	command_args.erase(command_args.begin());

	build_command();

	return command;
}

void CommandParser::split_command_text(std::string text)
{
	int pos;
	while ((pos = text.find(":")) != std::string::npos)
	{
		command_args.push_back(text.substr(0, pos));
		text.erase(0, pos + 1);
	}

	command_args.push_back(text);
}

void CommandParser::build_command()
{
	switch (type)
	{
	case CommandType::ADD:
		command = AddCommand::from_args(command_args, model);
	}
}
