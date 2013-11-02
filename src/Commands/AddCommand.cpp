/*
 * AddCommand.cpp
 *
 *  Created on: 2 lis 2013
 *      Author: Marcin Kolny
 */

#include "AddCommand.h"

AddCommand::AddCommand(AddRemoveType type,
		const Glib::RefPtr<Gst::Element>& parent,
		const Glib::RefPtr<Gst::Object>& object)
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
	if (type == AddRemoveType::PAD)
	{
		if (GST_IS_PAD(object->gobj()))
			parent->add_pad(object);
		else
			throw std::runtime_error("cannot run command: object is not a pad");
	}
	else
	{
		if (GST_IS_ELEMENT(object->gobj()) && GST_IS_PIPELINE(parent->gobj()))
		{
			Glib::RefPtr<Gst::Pipeline> pipeline = pipeline.cast_static(parent);
			pipeline->add(object);
		}
		else
			throw std::runtime_error("cannot run command: invalid parent or object type")
	}
}
