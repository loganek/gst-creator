/*
 * DisconnectCommand.h
 *
 *  Created on: 16 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef DISCONNECTCOMMAND_H_
#define DISCONNECTCOMMAND_H_

#include "ConnectCommand.h"
#include <gstreamermm.h>

class DisconnectCommand : public Command
{
private:
	Glib::RefPtr<Gst::Object> src;
	Glib::RefPtr<Gst::Object> dst;
	ObjectType type;
public:
	DisconnectCommand(const Glib::RefPtr<Gst::Object>& src, const Glib::RefPtr<Gst::Object>& dst);

	static DisconnectCommand* from_args(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);
	static std::vector<std::string> get_suggestions(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);
	void run_command(CommandListener* listener = nullptr);
};

#endif /* DISCONNECTCOMMAND_H_ */
