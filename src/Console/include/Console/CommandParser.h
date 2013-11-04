/*
 * CommandParser.h
 *
 *  Created on: 29 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef COMMANDPARSER_H_
#define COMMANDPARSER_H_

#include "Commands.h"
#include <string>
#include <gstreamermm.h>

class CommandParser
{
private:
	Command* command;
	CommandType type;
	std::vector<std::string> command_args;
	Glib::RefPtr<Gst::Pipeline> model;

	void build_command();
	void split_command_text(std::string text);
public:
	CommandParser(const Glib::RefPtr<Gst::Pipeline>& model);
	virtual ~CommandParser();

	Command* parse(const std::string& text);
};

#endif /* COMMANDPARSER_H_ */
