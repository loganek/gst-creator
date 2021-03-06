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
  value(value),
  caps_edit(nullptr)
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
	if (!value)
		value = Gst::Caps::create_any();
	tmp_value = value->to_string();
}


std::string PropertyCaps::get_str_value() const
{
	return value->to_string();
}

bool PropertyCaps::is_default_value() const
{
	const GValue* v = g_param_spec_get_default_value(param_spec);
	Glib::RefPtr<Gst::Caps> caps = Glib::wrap(const_cast<GstCaps*>(gst_value_get_caps(v)), false);

	return caps->equals(value);
}

std::string PropertyCaps::get_type_name() const
{
	return "Glib::RefPtr<Gst::Caps>";
}
