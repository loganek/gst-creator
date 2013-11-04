/*
 * AddCommand.cpp
 *
 *  Created on: 2 lis 2013
 *      Author: Marcin Kolny
 */

#include "AddCommand.h"
#include "enum_string_converter.h"
#include "utils/GstUtils.h"
#include <set>

using namespace Gst;
using Glib::RefPtr;

AddCommand::AddCommand(ObjectType type,
		const RefPtr<Element>& parent,
		const RefPtr<Object>& object)
: type(type),
  Command(CommandType::ADD),
  parent(parent),
  object(object)
{
}

AddCommand::~AddCommand()
{
}

void AddCommand::run_command()
{
	if (type == ObjectType::PAD)
	{
		if (GST_IS_PAD(object->gobj()))
		{
			RefPtr<Pad> pad = pad.cast_static(object);
			parent->add_pad(pad);
		}
		else
			throw std::runtime_error("cannot run command: object is not a pad");
	}
	else
	{
		if (GST_IS_ELEMENT(object->gobj()) && GST_IS_PIPELINE(parent->gobj()))
		{
			RefPtr<Pipeline> pipeline = pipeline.cast_static(parent);
			RefPtr<Element> element = element.cast_static(object);
			pipeline->add(element);
		}
		else
			throw std::runtime_error("cannot run command: invalid parent or object type");
	}
}

AddCommand* AddCommand::from_args(const std::vector<std::string>& args, const RefPtr<Pipeline>& model)
{
	std::set<int> allowed_args_count = {2, 3, 5};

	if (allowed_args_count.find(args.size()) == allowed_args_count.end())
	{
		std::string exp_args;
		int i = 0;
		for (auto it = allowed_args_count.begin(); it != allowed_args_count.end(); ++it)
		{
			exp_args += std::to_string(*it);
			i++;
			if (i == allowed_args_count.size() - 1)
			{
				exp_args += " or ";
				break;
			}
			exp_args += ",";
		}

		exp_args += std::to_string(*(allowed_args_count.rbegin()));

		syntax_error("invalid arguments count. Expected " + exp_args + ", but " +
				std::to_string(allowed_args_count.size()) + " found.");
	}

	ObjectType type = string_to_enum<ObjectType>(args[0]);

	if (type == ObjectType::ELEMENT)
	{
		RefPtr<Element> element =
				ElementFactory::create_element(args[1]);

		if (!element)
			throw std::runtime_error("Cannot find element " + args[1]);

		if (args.size() == 3 || args.size() == 5) // element's name defined
			element->set_name(args[2]);

		RefPtr<Element> parent;

		if (args.size() == 5)
		{
			if (args[3] != "to")
				syntax_error("expected `to`, but " + args[3] + " found.");

			parent = GstUtils::find_element(args[4], model);
		}
		else
		{
			parent = model;
		}

		return new AddCommand(type, parent, element);
	}
}
