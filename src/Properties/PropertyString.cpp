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
  value(value),
  edit(nullptr)
{
}

void PropertyString::set_value()
{
	element->property<Glib::ustring>(param_spec->name, value.c_str());
}

void PropertyString::build_widget()
{
	widget = new QWidget();
	widget->setLayout(new QHBoxLayout());
	widget->layout()->addWidget(new QLabel(param_spec->name));
	edit = new QLineEdit();
	widget->layout()->addWidget(edit);
	QObject::connect(edit, &QLineEdit::textChanged, this, &PropertyString::update_value);
	QObject::connect(edit, &QLineEdit::returnPressed, [&]{
		set_value();
		init();
	});
}

void PropertyString::init()
{
	Glib::ustring text;
	element->get_property(param_spec->name, text);
	edit->setText(text.c_str());
}

void PropertyString::update_value(const QString& new_value)
{
	value = new_value.toUtf8().constData();
}
