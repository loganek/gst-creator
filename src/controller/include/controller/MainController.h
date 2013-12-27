/*
 * MainController.h
 *
 *  Created on: 4 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef MAINCONTROLLER_H_
#define MAINCONTROLLER_H_

#include "Commands/CommandListener.h"
#include <gstreamermm.h>
#include <string>

class MainWindow;

class MainController : public CommandListener
{
private:
	Glib::RefPtr<Gst::Pipeline> model;
	std::string current_project_file;
	bool model_modified_state;
	MainWindow* main_view;

	void set_modified_state();

public:
	MainController(const Glib::RefPtr<Gst::Pipeline>& model);
	Glib::RefPtr<Gst::Pipeline> get_model() const;

	void set_main_view(MainWindow* main_view);

	void set_current_project_file(const std::string& current_project_file);
	std::string get_current_project_file() const;

	void reset_modified_state();
	bool get_modified_state() const;

	void pad_added(const Glib::RefPtr<Gst::Pad>& pad)
	{set_modified_state();}
	void pad_removed(const Glib::RefPtr<Gst::Pad>& pad)
	{set_modified_state();}
	void pad_linked(const Glib::RefPtr<Gst::Pad>& pad)
	{set_modified_state();}
	void pad_unlinked(const Glib::RefPtr<Gst::Pad>& pad)
	{set_modified_state();}
	void future_connection_added(const Glib::RefPtr<Gst::PadTemplate>& src_tpl,
			const Glib::RefPtr<Gst::Element>& parent, const Glib::RefPtr<Gst::Pad>& sink)
	{set_modified_state();}
	void future_connection_removed(const ConnectCommand::future_connection_pads& conn)
	{set_modified_state();}

	void clean_model();
};

#endif /* MAINCONTROLLER_H_ */
