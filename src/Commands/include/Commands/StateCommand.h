/*
 * StateCommand.h
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef STATECOMMAND_H_
#define STATECOMMAND_H_

#include "Command.h"
#include <gstreamermm.h>

class StateCommand : public Command
{
	State state;
	Glib::RefPtr<Gst::Pipeline> model;
public:
	StateCommand(State state, const Glib::RefPtr<Gst::Pipeline>& model);

	static StateCommand* from_args(const std::vector<std::string>& vect, const Glib::RefPtr<Gst::Pipeline>& model);
	static std::vector<std::string> get_suggestions(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);
	void run_command();
};

#endif /* STATECOMMAND_H_ */
