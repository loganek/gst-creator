/*
 * RemoveCommand.cpp
 *
 *  Created on: 16 lis 2013
 *      Author: Marcin Kolny
 */

#include "RemoveCommand.h"
#include "utils/EnumUtils.h"
#include "utils/GstUtils.h"

using namespace Gst;
using namespace std;
using Glib::RefPtr;

RemoveCommand::RemoveCommand(ObjectType type,
		const RefPtr<Object>& object)
: Command(CommandType::REMOVE),
  object(object),
  type(type)
{
}

RemoveCommand::~RemoveCommand()
{
}

void RemoveCommand::run_command(std::vector<CommandListener*> listeners)
{
	RefPtr<Object> parent = object->get_parent();

	if (type == ObjectType::ELEMENT)
	{
		RefPtr<Bin> bin = bin.cast_static(parent);
		bin->remove(RefPtr<Element>::cast_static(object));
	}
	else if (type == ObjectType::PAD)
	{
		RefPtr<Element> element = element.cast_static(parent);
		element->remove_pad(RefPtr<Pad>::cast_static(object));
	}
}

RemoveCommand* RemoveCommand::from_args(const vector<string>& args, const RefPtr<Pipeline>& model)
{
	if (args.size() != 2)
		syntax_error("invalid arguments count. Expected 2, but " + to_string(args.size()) + " found.");

	ObjectType type = EnumUtils<ObjectType>::string_to_enum(args[0]);

	switch (type)
	{
	case ObjectType::ELEMENT:
	{
		RefPtr<Element> element = GstUtils::find_element(args[1], model);
		return new RemoveCommand(type, element);
	}
	case ObjectType::PAD:
	{
		RefPtr<Pad> pad = GstUtils::find_pad(args[1], model);
		return new RemoveCommand(type, pad);
	}
	}

	throw runtime_error("Invalid element type: " + args[0]);
}

vector<string> RemoveCommand::get_suggestions(const vector<string>& args, const RefPtr<Pipeline>& model)
{
	try
	{
		ObjectType type;
		if (args.size() > 1)
			type = EnumUtils<ObjectType>::string_to_enum(args[0]);

		if (args.size() == 1)
			return EnumUtils<ObjectType>::get_string_values();
		else if (args.size() == 2 && type == ObjectType::ELEMENT)
			return GstUtils::get_elements_from_bin_string(model, false);
	}
	catch (...) {}
	return vector<string>();
}


