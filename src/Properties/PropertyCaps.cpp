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
	widget = new QWidget();
	widget->setLayout(new QHBoxLayout());
	widget->layout()->addWidget(new QLabel(param_spec->name));
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
	element->get_property(param_spec->name, value);
	tmp_value = value->to_string();
	caps_edit->setText(tmp_value.c_str());
}
