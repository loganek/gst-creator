/*
 * PropertyBoolean.cpp
 *
 *  Created on: 9 lis 2013
 *      Author: Marcin Kolny
 */

#include "PropertyBoolean.h"

PropertyBoolean::PropertyBoolean(GParamSpec* param_spec,
		const Glib::RefPtr<Gst::Element>& element,
		bool value)
: Property(param_spec, element),
  value(value)
{
}

void PropertyBoolean::set_value()
{
	element->property(param_spec->name, value);
}

void PropertyBoolean::build_widget()
{
	// TODO
}

void PropertyBoolean::init()
{
	// TODO
}
