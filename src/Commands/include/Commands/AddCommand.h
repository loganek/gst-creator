/*
 * AddCommand.h
 *
 *  Created on: 2 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef ADDCOMMAND_H_
#define ADDCOMMAND_H_

#include "Command.h"
#include <vector>
#include <gstreamermm.h>

class AddCommand : public Command
{
private:
	Glib::RefPtr<Gst::Object> object;
	Glib::RefPtr<Gst::Element> parent;
	ObjectType type;

	static AddCommand* generate_add_pad_command(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);
	static AddCommand* generate_add_element_command(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);
public:
	AddCommand(ObjectType type,
			const Glib::RefPtr<Gst::Element>& parent,
			const Glib::RefPtr<Gst::Object>& obj);
	virtual ~AddCommand();

	static AddCommand* from_args(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);
	static std::vector<std::string> get_suggestions(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);

	void run_command(CommandListener* listener = nullptr);
	Glib::RefPtr<Gst::Object> run_command_ret(CommandListener* listener = nullptr);

	Glib::RefPtr<Gst::Object> get_object() { return object; }
};

#endif /* ADDCOMMAND_H_ */
