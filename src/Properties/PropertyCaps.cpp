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

PropertyCaps::PropertyCaps(GParamSpec* param_spec,
		const Glib::RefPtr<Gst::Element>& element)
: Property(param_spec, element)
{
	read_var();
}

void PropertyCaps::set_value()
{
	element->property(param_spec->name, value);
}

void PropertyCaps::build_widget()
{
	caps_edit = new QLineEdit();
	widget->layout()->addWidget(caps_edit);
	QObject::connect(caps_edit, &QLineEdit::textChanged, [&](const QString& txt){
		tmp_value = txt.toUtf8().constData();
	});
	QObject::connect(caps_edit, &QLineEdit::returnPressed, [&]{
		value = Gst::Caps::create_from_string(tmp_value);
		set_value();
		init();
	});
}

void PropertyCaps::init()
{
	read_var();
	caps_edit->setText(tmp_value.c_str());
}

void PropertyCaps::read_var()
{
	element->get_property(param_spec->name, value);
	tmp_value = value->to_string();
}


std::string PropertyCaps::get_str_value() const
{
	return value->to_string().c_str();
}
