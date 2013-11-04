/*
 * CommandParser.cpp
 *
 *  Created on: 29 pa? 2013
 *      Author: Marcin Kolny
 */

#include "CommandParser.h"
#include "Commands/enum_string_converter.h"
#include "utils/StringUtils.h"
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
	command_args = StringUtils::split(StringUtils::trim(text), " ");

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

void CommandParser::build_command()
{
	switch (type)
	{
	case CommandType::ADD:
		command = AddCommand::from_args(command_args, model);
		break;
	case CommandType::STATE:
		command = StateCommand::from_args(command_args, model);
		break;
	}
}
