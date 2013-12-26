/*
 * MainController.cpp
 *
 *  Created on: 4 gru 2013
 *      Author: Marcin Kolny
 */

#include "MainController.h"

using Glib::RefPtr;
using namespace Gst;

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
