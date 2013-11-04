/*
 * GstUtils.cpp
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#include "GstUtils.h"
#include "StringUtils.h"

Glib::RefPtr<Gst::Element> GstUtils::find_element(std::string text, const Glib::RefPtr<Gst::Pipeline>& model)
{
	std::vector<std::string> elements = StringUtils::split(text, ":");
	Glib::RefPtr<Gst::Bin> current_bin = model;

	for (int i = 0; i < elements.size(); i++)
		current_bin = current_bin.cast_static(model->get_element(elements[i].c_str()));

	return current_bin->get_element(elements.back().c_str());
}

