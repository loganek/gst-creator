/*
 * MainController.cpp
 *
 *  Created on: 4 gru 2013
 *      Author: Marcin Kolny
 */

#include "MainController.h"
#include "gui/MainWindow.h"

using Glib::RefPtr;
using namespace Gst;

MainController::MainController(const RefPtr<Pipeline>& model)
: model(model),
  model_modified_state(false),
  main_view(nullptr)
{
	model->signal_element_added().connect([this](const Glib::RefPtr<Gst::Element>& e) {
		set_modified_state();
	});

	model->signal_element_removed().connect([this](const Glib::RefPtr<Gst::Element>& e) {
		set_modified_state();
	});

}

void MainController::set_main_view(MainWindow* main_view)
{
	this->main_view = main_view;
}

RefPtr<Pipeline> MainController::get_model() const
{
	return model;
}

void MainController::set_current_project_file(const std::string& current_project_file)
{
	this->current_project_file = current_project_file;

	main_view->current_project_file_changed(current_project_file);
}

std::string MainController::get_current_project_file() const
{
	return current_project_file;
}

void MainController::reset_modified_state()
{
	model_modified_state = false;

	if (main_view != nullptr)
		main_view->modified_state_changed(false);
}

void MainController::set_modified_state()
{
	Gst::State state, pending;
	model->get_state(state, pending, 0);

	if (state == Gst::STATE_NULL)
		model_modified_state = true;

	if (main_view != nullptr)
		main_view->modified_state_changed(model_modified_state);
}

bool MainController::get_modified_state() const
{
	return model_modified_state;
}
