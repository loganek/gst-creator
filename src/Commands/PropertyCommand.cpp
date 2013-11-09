/*
 * PropertyCommand.cpp
 *
 *  Created on: 9 lis 2013
 *      Author: Marcin Kolny
 */

#include "PropertyCommand.h"
#include "utils/EnumUtils.h"
#include "utils/GstUtils.h"

using namespace Gst;
using Glib::RefPtr;

PropertyCommand::PropertyCommand(const RefPtr<Element>& element,
		const std::string& property_name)
: element(element),
  property_name(property_name)
{}

PropertyCommand::~PropertyCommand()
{
}

void PropertyCommand::run_command()
{
}

PropertyCommand* PropertyCommand::from_args(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model)
{
	if (args.size() != 4)
		syntax_error("invalid arguments count. Expected 4, but " + std::to_string(args.size()) + " found.");

	RefPtr<Element> element = GstUtils::find_element(args[0], model);

	if (!element)
		throw std::runtime_error("cannot find element " + args[0]);

	DataType data_type;
	try
	{
		data_type = EnumUtils<DataType>::string_to_enum(args[2]);
	}
	catch (const std::runtime_error&)
	{
		syntax_error("unknown data type: " + args[2]);
	}

	GParamSpec* param =
			g_object_class_find_property(
					G_OBJECT_GET_CLASS(element->gobj()), args[1].c_str());

	if (param == nullptr)
		throw std::runtime_error("cannot find property " + args[1]);



	return nullptr;
}
