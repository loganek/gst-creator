/*
 * RemoveCommand.h
 *
 *  Created on: 16 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef REMOVECOMMAND_H_
#define REMOVECOMMAND_H_

#include "Command.h"
#include <vector>
#include <gstreamermm.h>

class RemoveCommand : public Command
{
private:
	Glib::RefPtr<Gst::Object> object;
	Glib::RefPtr<Gst::Element> parent;
	ObjectType type;

public:
	RemoveCommand(ObjectType type,
			const Glib::RefPtr<Gst::Object>& obj);
	virtual ~RemoveCommand();

	static RemoveCommand* from_args(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);
	static std::vector<std::string> get_suggestions(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);

	void run_command(CommandListener* listener = nullptr);
};

#endif /* REMOVECOMMAND_H_ */
