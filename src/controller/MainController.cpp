/*
 * MainController.cpp
 *
 *  Created on: 4 gru 2013
 *      Author: Marcin Kolny
 */

#include "MainController.h"

using Glib::RefPtr;
using namespace Gst;
#include <iostream>
void log_func(GstDebugCategory * category,
                                 GstDebugLevel      level,
                                 const gchar      * file,
                                 const gchar      * function,
                                 gint               line,
                                 GObject          * object,
                                 GstDebugMessage  * message,
                                 gpointer           user_data)
{
	std::cout << gst_debug_message_get(message) << std::endl;
}

MainController::MainController(const RefPtr<Pipeline>& model)
: model(model),
  model_modified_state(false)
{
	model->signal_element_added().connect([this](const Glib::RefPtr<Gst::Element>& e) {
		set_modified_state();
	});

	model->signal_element_removed().connect([this](const Glib::RefPtr<Gst::Element>& e) {
		set_modified_state();
	});

	gst_debug_set_default_threshold(GST_LEVEL_INFO);
	gst_debug_add_log_function(log_func, nullptr, nullptr);
}

RefPtr<Pipeline> MainController::get_model() const
{
	return model;
}

void MainController::set_current_project_file(const std::string& current_project_file)
{
	this->current_project_file = current_project_file;
}

std::string MainController::get_current_project_file() const
{
	return current_project_file;
}

void MainController::reset_modified_state()
{
	model_modified_state = false;
}

void MainController::set_modified_state()
{
	Gst::State state, pending;
	model->get_state(state, pending, 0);

	if (state == Gst::STATE_NULL)
		model_modified_state = true;
}

bool MainController::get_modified_state() const
{
	return model_modified_state;
}

void MainController::set_debug_level(int level)
{
	if (level < 0) level = 0;
	else if (level > GST_LEVEL_TRACE) level = GST_LEVEL_TRACE;

	gst_debug_set_default_threshold((GstDebugLevel)level);
}
