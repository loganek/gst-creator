/*
 * DisconnectCommand.cpp
 *
 *  Created on: 16 lis 2013
 *      Author: Marcin Kolny
 */

#include "DisconnectCommand.h"
#include "utils/EnumUtils.h"
#include "utils/GstUtils.h"

using namespace Gst;
using Glib::RefPtr;
using namespace std;

DisconnectCommand::DisconnectCommand(const RefPtr<Object>& src, const RefPtr<Object>& dst)
: src(src),
  dst(dst),
  Command(CommandType::DISCONNECT)
{
	if (GST_IS_ELEMENT(src->gobj()) || GST_IS_ELEMENT(dst->gobj()))
		type = ObjectType::ELEMENT;
	else if (GST_IS_PAD(src->gobj()) || GST_IS_PAD(dst->gobj()))
		type = ObjectType::PAD;
	else
		syntax_error("unknown object type");
}

DisconnectCommand* DisconnectCommand::from_args(const vector<string>& args, const Glib::RefPtr<Gst::Pipeline>& model)
{
	if (args.size() != 4)
		syntax_error("invalid arguments count. Expected 4, but " + to_string(args.size()) + " found.");

	if (!StringUtils::are_equal_case_no_sense(args[2], "with"))
		syntax_error("expected `WITH`, but " + args[2] + " found.");

	ObjectType type = EnumUtils<ObjectType>::string_to_enum(args[0]);

	if (type == ObjectType::ELEMENT)
	{
		RefPtr<Element> src = GstUtils::find_element(args[1], model),
				dest = GstUtils::find_element(args[3], model);

		return new DisconnectCommand(src, dest);
	}
	else
	{
		RefPtr<Pad> src = GstUtils::find_pad(args[1], model),
				dest = GstUtils::find_pad(args[3], model);

		return new DisconnectCommand(src, dest);
	}
}

void DisconnectCommand::run_command()
{
	if (type == ObjectType::ELEMENT)
	{
		RefPtr<Element> e_src = e_src.cast_static(src),
				e_dst = e_dst.cast_static(dst);
		e_src->unlink(e_dst);
	}
	else
	{
		RefPtr<Pad> p_src = p_src.cast_static(src),
				p_dst = p_dst.cast_static(dst);
		p_src->unlink(p_dst);
	}
}

vector<string> DisconnectCommand::get_suggestions(const vector<string>& args, const RefPtr<Pipeline>& model)
{
	try
	{
		ObjectType type;
		if (args.size() > 1)
			type = EnumUtils<ObjectType>::string_to_enum(args[0]);

		if (args.size() == 1)
			return EnumUtils<ObjectType>::get_string_values();
		else if ((args.size() == 2 || args.size() == 4) && type == ObjectType::ELEMENT)
			return GstUtils::get_elements_from_bin_string(model, false);
		else if ((args.size() == 2 || args.size() == 4) && type == ObjectType::PAD)
			return vector<string>(); // TODO
		else if (args.size() == 3)
			return {"WITH"};
	}
	catch (...) {}

	return vector<string>();
}

