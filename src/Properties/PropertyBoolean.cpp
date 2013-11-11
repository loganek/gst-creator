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

void PropertyBoolean::set_value()
{
	element->property(param_spec->name, value);
}

void PropertyBoolean::build_widget()
{
	widget = new QWidget();
	widget->setLayout(new QHBoxLayout());
	widget->layout()->addWidget(new QLabel(param_spec->name));
	checkbox = new QCheckBox();
	widget->layout()->addWidget(checkbox);
	QObject::connect(checkbox, &QCheckBox::stateChanged, [&](int state){
		value = !!state;
	});
}

void PropertyBoolean::init()
{
	element->get_property<bool>(param_spec->name, value);
	checkbox->setChecked(value);

}
