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
public:
	typedef std::pair<Glib::RefPtr<Gst::Element>, Glib::RefPtr<Gst::PadTemplate>> template_parent;
	typedef std::pair<template_parent, Glib::RefPtr<Gst::Pad>> future_connection_pads;
	typedef std::pair<Glib::RefPtr<Gst::Element>, Glib::RefPtr<Gst::Element>> future_connection_elements;
private:
	Glib::RefPtr<Gst::Object> src;
	Glib::RefPtr<Gst::Object> dst;
	Glib::RefPtr<Gst::Element> src_parent;
	bool future;
	ObjectType type;

	static std::set<future_connection_pads> future_connection_pads_map;
	static std::set<future_connection_elements> future_connection_elements_map;

	static std::string future_keyword;
	void connect_future_pads(const Glib::RefPtr<Gst::Element>& src_parent, const Glib::RefPtr<Gst::PadTemplate>& src_pad, const Glib::RefPtr<Gst::Pad>& sink_pad);
	void connect_future_elements(const Glib::RefPtr<Gst::Element>& src, const Glib::RefPtr<Gst::Element>& sink);
public:
	ConnectCommand(const Glib::RefPtr<Gst::Object>& src, const Glib::RefPtr<Gst::Object>& dst, bool future = false);
	ConnectCommand(const Glib::RefPtr<Gst::PadTemplate>& src, const Glib::RefPtr<Gst::Element>& src_parent, const Glib::RefPtr<Gst::Pad>& dst);

	ObjectType get_type() { return type; }
	Glib::RefPtr<Gst::Object> get_src() { return src; }
	Glib::RefPtr<Gst::Object> get_dst() { return dst; }
	static ConnectCommand* from_args(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);
	static std::vector<std::string> get_suggestions(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);
	void run_command(CommandListener* listener = nullptr);

	static void element_pad_added(const Glib::RefPtr<Gst::Pad>& pad);
	static void remove_future_connection(const Glib::RefPtr<Gst::PadTemplate>& tpl, const Glib::RefPtr<Gst::Pad>& pad, CommandListener* listener);

	static std::set<future_connection_elements> get_future_connections_element();
	static std::set<future_connection_pads> get_future_connections_pads();
};

#endif /* CONNECTCOMMAND_H_ */
