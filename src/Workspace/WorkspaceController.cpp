/*
 * WorkspaceController.cpp
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#include "WorkspaceController.h"

using namespace std;
using namespace Gst;
using Glib::RefPtr;

WorkspaceController::WorkspaceController()
{
}

WorkspaceController::~WorkspaceController()
{
}

void WorkspaceController::add_element(const string& factory_name, const string& element_name)
{
	RefPtr<Element> element = ElementFactory::create_element(factory_name.c_str(), element_name.c_str());
	model->add(element);
}

void WorkspaceController::remove_element(const string& element_name)
{
	RefPtr<Element> element = model->get_element(element_name.c_str());
	model->remove(element);
}

void WorkspaceController::link_element(const string& source_element, const string& source_pad,
		const string& destination_element, const string& destination_pad)
{
	RefPtr<Element> source = model->get_element(source_element.c_str()),
			destination = model->get_element(destination_element.c_str());

	source->link_pads(source_pad.c_str(), destination, destination_element.c_str());
}

void WorkspaceController::unlink_element(const string& source_element, const string& source_pad,
		const string& destination_element, const string& destination_pad)
{
	RefPtr<Element> source = model->get_element(source_element.c_str()),
			destination = model->get_element(destination_element.c_str());

	source->unlink_pads(source_pad.c_str(), destination, destination_element.c_str());
}
