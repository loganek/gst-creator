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
  value(value),
  checkbox(nullptr)
{
}

PropertyBoolean::PropertyBoolean(GParamSpec* param_spec,
		const Glib::RefPtr<Gst::Element>& element)
: Property(param_spec, element),
  checkbox(nullptr)
{
	read_var();
}

void PropertyBoolean::set_value()
{
	element->property(param_spec->name, value);
}

void PropertyBoolean::build_widget()
{
	checkbox = new QCheckBox();
	widget->layout()->addWidget(checkbox);
	QObject::connect(checkbox, &QCheckBox::stateChanged, [&](int state){
		value = !!state;
	});
}

void PropertyBoolean::init()
{
	read_var();
	checkbox->setChecked(value);

}

void PropertyBoolean::read_var()
{
	element->get_property<bool>(param_spec->name, value);
}

std::string PropertyBoolean::get_str_value() const
{
	return std::to_string(value);
}
