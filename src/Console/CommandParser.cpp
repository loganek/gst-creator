/*
 * CommandParser.cpp
 *
 *  Created on: 29 pa? 2013
 *      Author: Marcin Kolny
 */

#include "CommandParser.h"
#include "utils/EnumUtils.h"
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
 *    - element factory [_name_] [to bin1:bin2:bin3 default=pipeline]
 *    - pad using _template_ [_name_] to [bin1:bin2 default=pipeline]:element
 *
 *  - remove
 *    - element
 *    - pad
 *
 *  - connect
 *    - {element|pad} obj1 with obj2
 *    - pads element1:pad1 element2:pad2
 *  - reconnect
 *    - {pad | element}
 *    - to {pad | element}
 *  - property
 *    - element property_name TYPE value
 *
 */
Command* CommandParser::parse(const std::string& text)
{
	command_args = StringUtils::split(StringUtils::trim(text), " ");

	try
	{
		type = EnumUtils<CommandType>::string_to_enum(command_args[0]);
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
	case CommandType::CONNECT:
		command = ConnectCommand::from_args(command_args, model);
		break;
	case CommandType::PROPERTY:
		command = PropertyCommand::from_args(command_args, model);
		break;
	case CommandType::REMOVE:
		command = RemoveCommand::from_args(command_args, model);
		break;
	case CommandType::DISCONNECT:
		command = DisconnectCommand::from_args(command_args, model);
		break;
	}
}
