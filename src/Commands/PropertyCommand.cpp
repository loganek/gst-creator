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
#include <memory>

using namespace Gst;
using Glib::RefPtr;

PropertyCommand::PropertyCommand(const RefPtr<Element>& element,
		const std::string& property_name, const std::string& property_value)
{
	GParamSpec* param = g_object_class_find_property(
			G_OBJECT_GET_CLASS(element->gobj()), property_name.c_str());

	property = Property::build_property(param, element, property_value);
}

PropertyCommand::~PropertyCommand()
{
	delete property;
}

void PropertyCommand::run_command()
{
	property->set_value();
}

PropertyCommand* PropertyCommand::from_args(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model)
{
	if (args.size() != 3)
		syntax_error("invalid arguments count. Expected 3, but " + std::to_string(args.size()) + " found.");

	RefPtr<Element> element = GstUtils::find_element(args[0], model);

	if (!element)
		throw std::runtime_error("cannot find element " + args[0]);

	return new PropertyCommand(element, args[1], args[2]);
}
