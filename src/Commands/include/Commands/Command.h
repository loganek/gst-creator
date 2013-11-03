/*
 * Command.h
 *
 *  Created on: 2 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <stdexcept>

enum class CommandType
{
	ADD,
	REMOVE,
	CONNECT,
	DISCONNECT,
	RECONNECT,
	UNKNOW
};

enum class AddRemoveType
{
	PAD,
	ELEMENT
};

class Command
{
private:
	CommandType type;

public:
	Command(CommandType type):type(type){}
	Command():type(CommandType::UNKNOW){}
	virtual ~Command(){}

	CommandType get_type()
	{
		return type;
	}

	virtual void run_command() = 0;

	static void syntax_error(const std::string& error)
	{
		throw std::runtime_error("Syntax error: " + error);
	}

};

#endif /* COMMAND_H_ */
