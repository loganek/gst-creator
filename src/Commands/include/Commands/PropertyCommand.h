/*
 * PropertyCommand.h
 *
 *  Created on: 9 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef PROPERTYCOMMAND_H_
#define PROPERTYCOMMAND_H_



#include "Command.h"
#include "Properties/Property.h"
#include <vector>
#include <gstreamermm.h>

class PropertyCommand : public Command
{
private:
	Property* property;
	bool run_window;
	Glib::RefPtr<Gst::Element> element;

public:
	PropertyCommand(
			const Glib::RefPtr<Gst::Element>& element,
			const std::string& property_name,
			const std::string& property_value);
	virtual ~PropertyCommand();

	static PropertyCommand* from_args(const std::vector<std::string>& vect, const Glib::RefPtr<Gst::Pipeline>& model);
	static std::vector<std::string> get_suggestions(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);
	void run_command();
};


#endif /* PROPERTYCOMMAND_H_ */
