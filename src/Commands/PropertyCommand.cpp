/*
 * PropertyCommand.cpp
 *
 *  Created on: 9 lis 2013
 *      Author: Marcin Kolny
 */

#include "PropertyCommand.h"
#include "utils/EnumUtils.h"
#include "utils/GstUtils.h"
#include "Properties/Property.h"

using namespace Gst;
using Glib::RefPtr;
using namespace std;

PropertyCommand::PropertyCommand(const RefPtr<Element>& element,
		const string& property_name, const string& property_value)
: element(element),
  Command(CommandType::PROPERTY)
{
	run_window = property_name.empty();

	if (run_window)
		property = nullptr;
	else
	{
		GParamSpec* param = g_object_class_find_property(
				G_OBJECT_GET_CLASS(element->gobj()), property_name.c_str());

		property = Property::build_property(param, element, property_value);
	}
}

PropertyCommand::~PropertyCommand()
{
	delete property;
}

void PropertyCommand::run_command(std::vector<CommandListener*> listeners)
{
	if (run_window)
	{
		Property::build_property_window(element)->show();
	}
	else if (property != nullptr)
		property->set_value();
	else
		throw runtime_error("Cannot set property. Property unavailable.");
}

PropertyCommand* PropertyCommand::from_args(const vector<string>& args, const RefPtr<Pipeline>& model)
{
	if (args.size() != 3 && args.size() != 1)
		syntax_error("invalid arguments count. Expected 1 or 3, but " + to_string(args.size()) + " found.");

	RefPtr<Element> element = GstUtils::find_element(args[0], model);

	if (!element)
		throw runtime_error("cannot find element " + args[0]);

	if (args.size() == 1)
		return new PropertyCommand(element, "", "");

	return new PropertyCommand(element, args[1], args[2]);
}

vector<string> PropertyCommand::get_suggestions(const vector<string>& args, const RefPtr<Pipeline>& model)
{
	if (args.size() == 1)
		return GstUtils::get_elements_from_bin_string(model, false);
	if (args.size() == 2)
		return GstUtils::get_properties_string(GstUtils::find_element(args[0], model));
	return vector<string>();
}

