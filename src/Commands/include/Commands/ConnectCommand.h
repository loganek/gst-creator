/*
 * ConnectCommand.h
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef CONNECTCOMMAND_H_
#define CONNECTCOMMAND_H_

#include "Command.h"
#include <gstreamermm.h>

class ConnectCommand : public Command
{
private:
	Glib::RefPtr<Gst::Object> src;
	Glib::RefPtr<Gst::Object> dst;
	ObjectType type;
public:
	ConnectCommand(const Glib::RefPtr<Gst::Object>& src, const Glib::RefPtr<Gst::Object>& dst);

	static ConnectCommand* from_args(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);

	void run_command();
};

#endif /* CONNECTCOMMAND_H_ */
