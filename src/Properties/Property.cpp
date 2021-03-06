/*
 * Property.cpp
 *
 *  Created on: 9 lis 2013
 *      Author: Marcin Kolny
 */

#include "Properties.h"
#include "utils/GstUtils.h"
#include "utils/StringUtils.h"

using namespace Gst;
using Glib::RefPtr;

Property::Property(GParamSpec* param_spec, const RefPtr<Element>& element)
: was_built(false),
  param_spec(param_spec),
  element(element)
{
	widget = new QWidget();
	widget->setLayout(new QHBoxLayout());
	widget->layout()->addWidget(new QLabel(param_spec->name));
}

Property::~Property()
{
	delete widget;
}

QWidget* Property::get_widget()
{
	if (!was_built)
	{
		build_widget();
		was_built = true;
	}

	init();

	return widget;
}

bool Property::is_writable() const
{
	return param_spec->flags & G_PARAM_WRITABLE;
}

Property* Property::build_property(GParamSpec* param_spec,
		const RefPtr<Element>& element, const std::string& str_value)
{
	GValue value = { 0, };
	g_value_init (&value, param_spec->value_type);
	GType value_type = G_VALUE_TYPE(&value);

	if (GstUtils::is_numeric_type(value_type))
		return build_numeric_property(param_spec, element, value_type, str_value);
	else if( G_IS_PARAM_SPEC_ENUM (param_spec))
		return str_value.empty() ? new PropertyEnum(param_spec, element) :
				new PropertyEnum(param_spec, element, str_value);
	else if (!strcmp("GstCaps", g_type_name(value_type)))
		return str_value.empty() ? new PropertyCaps(param_spec, element):
				new PropertyCaps(param_spec, element,
						Caps::create_from_string(str_value.c_str()));
	else
	{
		switch (value_type)
		{
		case G_TYPE_BOOLEAN:
			return str_value.empty() ?
					new PropertyBoolean(param_spec, element):
					new PropertyBoolean(param_spec, element,
							StringUtils::str_to_numeric<bool>(str_value));
		case G_TYPE_STRING:
			return str_value.empty() ? new PropertyString(param_spec, element) :
					new PropertyString(param_spec, element, str_value);
		default:
			return nullptr;
		}
	}
}

Property* Property::build_numeric_property(GParamSpec* param_spec,
		const RefPtr<Element>& element, GType type, const std::string& value)
{
#define NUM_CASE(g_type, type) case g_type: \
		return value.empty() ? \
				new PropertyNumeric<type>(param_spec, element): \
				new PropertyNumeric<type>(param_spec, element, \
						StringUtils::str_to_numeric<type>(value));

	switch (type)
	{
	NUM_CASE(G_TYPE_ULONG, unsigned long)
	NUM_CASE(G_TYPE_LONG, long)
	NUM_CASE(G_TYPE_INT, int)
	NUM_CASE(G_TYPE_UINT, unsigned int)
	NUM_CASE(G_TYPE_UINT64, guint64)
	NUM_CASE(G_TYPE_INT64, gint64)
	NUM_CASE(G_TYPE_DOUBLE, double)
	NUM_CASE(G_TYPE_FLOAT, float)
	}

	return nullptr;
}

QWidget* Property::build_property_window(const RefPtr<Element>& element)
{
	guint property_count;
	PropertyWidget* widget = new PropertyWidget();
	QScrollArea* scroll_area = new QScrollArea();

	GParamSpec **property_specs = g_object_class_list_properties(
			G_OBJECT_GET_CLASS(element->gobj()), &property_count);

	for (size_t i = 0; i < property_count; i++)
	{
		Property* property = build_property(property_specs[i], element, "");
		if (property != nullptr)
			widget->add_property(property);
	}

	scroll_area->setWidget(widget);

	return scroll_area;
}
