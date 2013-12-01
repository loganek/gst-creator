/*
 * ConnectCommand.h
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef CONNECTCOMMAND_H_
#define CONNECTCOMMAND_H_

#include "Command.h"
#include <gstreamermm.h>
#include <set>
#include <utility>

class ConnectCommand : public Command
{
private:
	Glib::RefPtr<Gst::Object> src;
	Glib::RefPtr<Gst::Object> dst;
	bool future;
	ObjectType type;

	typedef std::pair<Glib::RefPtr<Gst::PadTemplate>, Glib::RefPtr<Gst::Pad>> future_connection_pads;
	typedef std::pair<Glib::RefPtr<Gst::Element>, Glib::RefPtr<Gst::Element>> future_connection_elements;
	static std::set<future_connection_pads> future_connection_pads_map;
	static std::set<future_connection_elements> future_connection_elements_map;

	static std::string future_keyword;
	void connect_future_pads(const Glib::RefPtr<Gst::PadTemplate>& src_pad, const Glib::RefPtr<Gst::Pad>& sink_pad);
	void connect_future_elements(const Glib::RefPtr<Gst::Element>& src_pad, const Glib::RefPtr<Gst::Element>& sink_pad);
public:
	ConnectCommand(const Glib::RefPtr<Gst::Object>& src, const Glib::RefPtr<Gst::Object>& dst, bool future = false);

	ObjectType get_type() { return type; }
	Glib::RefPtr<Gst::Object> get_src() { return src; }
	Glib::RefPtr<Gst::Object> get_dst() { return dst; }
	static ConnectCommand* from_args(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);
	static std::vector<std::string> get_suggestions(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);
	void run_command(CommandListener* listener = nullptr);

	static void element_pad_added(const Glib::RefPtr<Gst::Pad>& pad);
};

#endif /* CONNECTCOMMAND_H_ */
