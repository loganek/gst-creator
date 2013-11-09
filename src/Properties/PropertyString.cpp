/*
 * PropertyString.cpp
 *
 *  Created on: 9 lis 2013
 *      Author: Marcin Kolny
 */

#include "PropertyString.h"

PropertyString::PropertyString(GParamSpec* param_spec,
		const Glib::RefPtr<Gst::Element>& element,
		const std::string& value)
: Property(param_spec, element),
  value(value)
{
}

void PropertyString::set_value()
{
	element->property<Glib::ustring>(param_spec->name, value.c_str());
}

void PropertyString::build_widget()
{
	// TODO
}

