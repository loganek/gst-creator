/*
 * ConnectCommand.cpp
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#include "ConnectCommand.h"
#include "utils/EnumUtils.h"
#include "utils/GstUtils.h"

using namespace Gst;
using Glib::RefPtr;

ConnectCommand::ConnectCommand(const RefPtr<Object>& src, const RefPtr<Object>& dst)
: src(src),
  dst(dst)
{
	if (GST_IS_ELEMENT(src->gobj()) || GST_IS_ELEMENT(dst->gobj()))
		type = ObjectType::ELEMENT;
	else if (GST_IS_PAD(src->gobj()) || GST_IS_PAD(dst->gobj()))
		type = ObjectType::PAD;
	else
		syntax_error("unknown object type");
}

ConnectCommand* ConnectCommand::from_args(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model)
{
	if (args.size() != 4)
		syntax_error("invalid arguments count. Expected 4, but " + std::to_string(args.size()) + " found.");

	if (args[2] != "with")
		syntax_error("expected `with`, but " + args[2] + " found.");

	ObjectType type = EnumUtils<ObjectType>::string_to_enum(args[0]);

	if (type == ObjectType::ELEMENT)
	{
		RefPtr<Element> src = GstUtils::find_element(args[1], model),
				dest = GstUtils::find_element(args[3], model);

		return new ConnectCommand(src, dest);
	}
	else
	{
		RefPtr<Pad> src = GstUtils::find_pad(args[1], model),
				dest = GstUtils::find_pad(args[3], model);

		return new ConnectCommand(src, dest);
	}
}

void ConnectCommand::run_command()
{
	if (type == ObjectType::ELEMENT)
	{
		RefPtr<Element> e_src = e_src.cast_static(src),
				e_dst = e_dst.cast_static(dst);
		e_src->link(e_dst);
	}
	else
	{
		RefPtr<Pad> p_src = p_src.cast_static(src),
				p_dst = p_dst.cast_static(dst);
		p_src->link(p_dst);
	}
}

