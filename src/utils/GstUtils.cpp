/*
 * GstUtils.cpp
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#include "GstUtils.h"
#include "StringUtils.h"

using namespace Gst;
using Glib::RefPtr;

RefPtr<Gst::Element> GstUtils::find_element(std::string text, const RefPtr<Pipeline>& model)
{
	std::vector<std::string> elements = StringUtils::split(text, ":");
	RefPtr<Bin> current_bin = model;

	for (int i = 0; i < elements.size() - 1; i++)
		current_bin = current_bin.cast_static(model->get_element(elements[i].c_str()));

	return current_bin->get_element(elements.back().c_str());
}

RefPtr<Pad> GstUtils::find_pad(std::string text, const RefPtr<Pipeline>& model)
{
	unsigned pos = text.find_last_of(":");
	RefPtr<Element> element = find_element(text.substr(0, pos), model);
	std::string pad_name = text.substr(pos + 1);

	return element->get_static_pad(pad_name.c_str());
}
