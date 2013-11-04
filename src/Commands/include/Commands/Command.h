/*
 * Command.h
 *
 *  Created on: 2 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <stdexcept>
#include <vector>
#include <gstreamermm.h>

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

	static Glib::RefPtr<Gst::Element> find_element(std::string text, const Glib::RefPtr<Gst::Pipeline>& model)
	{
		int pos;
		std::vector<std::string> elements;

		while ((pos = text.find(":")) != std::string::npos)
		{
			elements.push_back(text.substr(0, pos));
			text.erase(0, pos + 1);
		}

		Glib::RefPtr<Gst::Bin> current_bin = model;

		for (int i = 0; i < elements.size(); i++)
		{
			current_bin = current_bin.cast_static(model->get_element(elements[i].c_str()));
		}

		return current_bin->get_element(elements.back().c_str());
	}

};

#endif /* COMMAND_H_ */
