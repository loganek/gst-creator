/*
 * PropertyCaps.cpp
 *
 *  Created on: 10 lis 2013
 *      Author: Marcin Kolny
 */

#include "PropertyCaps.h"

PropertyCaps::PropertyCaps(GParamSpec* param_spec,
		const Glib::RefPtr<Gst::Element>& element,
		const Glib::RefPtr<Gst::Caps>& value)
: Property(param_spec, element),
  value(value)
{
}

void PropertyCaps::set_value()
{
	element->property(param_spec->name, value);
}

void PropertyCaps::build_widget()
{
	// TODO
}

void PropertyCaps::init()
{
	// TODO
}
