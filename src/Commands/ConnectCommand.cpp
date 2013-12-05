/*
 * ConnectCommand.cpp
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#include "utils/EnumUtils.h"
#include "utils/GstUtils.h"
#include "CommandListener.h"
#include <set>

using namespace Gst;
using Glib::RefPtr;
using namespace std;

string ConnectCommand::future_keyword = "FUTURE";
std::set<ConnectCommand::future_connection_pads> ConnectCommand::future_connection_pads_map;
std::set<ConnectCommand::future_connection_elements> ConnectCommand::future_connection_elements_map;

std::set<ConnectCommand::future_connection_elements> ConnectCommand::get_future_connections_element()
{
	return future_connection_elements_map;
}

std::set<ConnectCommand::future_connection_pads> ConnectCommand::get_future_connections_pads()
{
	return future_connection_pads_map;
}

void ConnectCommand::connect_future_elements(const RefPtr<Element>& src, const RefPtr<Element>& sink)
{
	future_connection_elements_map.insert(future_connection_elements(src, sink));
}

void ConnectCommand::connect_future_pads(const RefPtr<Element>& src_parent, const RefPtr<PadTemplate>& src_pad, const RefPtr<Pad>& sink_pad)
{
	future_connection_pads_map.insert(future_connection_pads(template_parent(src_parent, src_pad), sink_pad));
}

void ConnectCommand::element_pad_added(const RefPtr<Pad>& pad)
{
	// TODO check template pad

	// check, is pad->pad connection defined
	auto tpl = pad->get_pad_template();
	for (auto it = future_connection_pads_map.begin(); it != future_connection_pads_map.end(); ++it)
	{
		if (it->first.second->get_parent() == tpl->get_parent() &&
				it->first.second->get_name() == tpl->get_name() &&
				pad->can_link(it->second))
		{
			pad->link(it->second);
			return;
		}
	}

	// check, element->element connection defined
	auto parent = pad->get_parent();
	for (auto it = future_connection_elements_map.begin(); it != future_connection_elements_map.end(); ++it)
	{
		if (it->first->get_parent() == parent->get_parent() && it->first->get_name() == it->first->get_name())
		{
			typedef Gst::Iterator<Gst::Pad> PadIter;

			PadIter iter = it->second->iterate_pads();
			while(iter.next())
			{
				if (pad->can_link(*iter))
				{
					pad->link(*iter);
					return;
				}
			}

			auto tpls = it->second->get_factory()->get_static_pad_templates();

			for(auto iter = tpls.begin();
					iter != tpls.end(); ++iter )
			{
				if (iter->get()->get_presence() != PAD_REQUEST)
					continue;

				auto new_pad = Pad::create(iter->get());
				pad->can_link(new_pad);
				it->second->add_pad(new_pad);
				pad->link(new_pad);

				return;
			}
		}
	}
}

ConnectCommand::ConnectCommand(const RefPtr<Object>& src, const RefPtr<Object>& dst, bool future)
: src(src),
  dst(dst),
  Command(CommandType::CONNECT),
  future(future)
{
	if (GST_IS_ELEMENT(src->gobj()) || GST_IS_ELEMENT(dst->gobj()))
	{
		type = ObjectType::ELEMENT;
		if (future)
			connect_future_elements(RefPtr<Element>::cast_static(src), RefPtr<Element>::cast_static(dst));
	}
	else if (GST_IS_PAD(src->gobj()) || GST_IS_PAD(dst->gobj()))
	{
		type = ObjectType::PAD;
		if (future)
			syntax_error("invalid constructor used"); // fixme
	}
	else
		syntax_error("unknown object type");
}

ConnectCommand::ConnectCommand(const RefPtr<PadTemplate>& src, const RefPtr<Element>& src_parent, const RefPtr<Pad>& dst)
: src(src),
  dst(dst),
  Command(CommandType::CONNECT),
  src_parent(src_parent),
  future(true)
{
	if (GST_IS_PAD(src->gobj()) || GST_IS_PAD(dst->gobj()))
	{
		type = ObjectType::PAD;
		connect_future_pads(src_parent, RefPtr<PadTemplate>::cast_static(src), RefPtr<Pad>::cast_static(dst));
	}
	else
		syntax_error("unknown object type");
}

void ConnectCommand::remove_future_connection(const RefPtr<PadTemplate>& tpl, const RefPtr<Pad>& pad, CommandListener* listener)
{
	for (auto it = future_connection_pads_map.begin(); it != future_connection_pads_map.end(); ++it)
	{
		if (it->first.second == tpl && it->second == pad)
		{
			listener->future_connection_removed(*it);
			future_connection_pads_map.erase(it);
		}
	}
}

ConnectCommand* ConnectCommand::from_args(const vector<string>& args, const Glib::RefPtr<Gst::Pipeline>& model)
{
	if (args.size() != 4 && args.size() != 5)
		syntax_error("invalid arguments count. Expected 4 or 5, but " + to_string(args.size()) + " found.");

	if (!StringUtils::are_equal_case_no_sense(args[2], "with"))
		syntax_error("expected `WITH`, but " + args[2] + " found.");

	ObjectType type = EnumUtils<ObjectType>::string_to_enum(args[0]);

	bool future = args.size() == 5 && StringUtils::are_equal_case_no_sense(future_keyword, args[4]);

	if (type == ObjectType::ELEMENT)
	{
		RefPtr<Element> src = GstUtils::find_element(args[1], model),
				dest = GstUtils::find_element(args[3], model);

		return new ConnectCommand(src, dest, future);
	}
	else
	{
		RefPtr<Object> src;

		if (future)
			src = GstUtils::find_pad_template(args[1], model);
		else
			src = GstUtils::find_pad(args[1], model);

		RefPtr<Pad> dest = GstUtils::find_pad(args[3], model);

		return new ConnectCommand(src, dest, future);
	}
}

void ConnectCommand::run_command(CommandListener* listener)
{
	if (type == ObjectType::ELEMENT)
	{
		RefPtr<Element> e_src = e_src.cast_static(src),
				e_dst = e_dst.cast_static(dst);

		if (future)
			e_src->signal_pad_added().connect(sigc::ptr_fun(&ConnectCommand::element_pad_added));
		else
			e_src->link(e_dst);
	}
	else
	{
		if (future)
		{
			RefPtr<PadTemplate> p_src = p_src.cast_static(src);
			src_parent->signal_pad_added().connect(sigc::ptr_fun(&ConnectCommand::element_pad_added));
			listener->future_connection_added(p_src, src_parent, RefPtr<Pad>::cast_static(dst));
		}
		else
		{
			RefPtr<Pad> p_src = p_src.cast_static(src);
			RefPtr<Pad> p_dst = p_dst.cast_static(dst);
			p_src->link(p_dst);
		}
	}
}

vector<string> ConnectCommand::get_suggestions(const vector<string>& args, const RefPtr<Pipeline>& model)
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
		else if (args.size() == 2 && type == ObjectType::PAD)
		{
			auto a = GstUtils::get_elements_from_bin_string(model, false);
			auto b = GstUtils::get_elements_from_bin_string(model, false);
			std::vector<std::string> sum;
			sum.reserve(a.size() + b.size());
			sum.insert(sum.end(), a.begin(), a.end());
			sum.insert(sum.end(), b.begin(), b.end());


			return GstUtils::get_pads_from_bin_string(model);
		}
		else if (args.size() == 4 && type == ObjectType::PAD)
			return GstUtils::get_pads_from_bin_string(model);
		else if (args.size() == 3)
			return {"WITH"};
		else if (args.size() == 5)
			return {future_keyword};
	}
	catch (...) {}

	return vector<string>();
}


