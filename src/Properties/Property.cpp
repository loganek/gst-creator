/*
 * Property.cpp
 *
 *  Created on: 9 lis 2013
 *      Author: Marcin Kolny
 */

#include "Properties.h"
#include "utils/GstUtils.h"
#include "utils/StringUtils.h"

Property::Property(GParamSpec* param_spec, const Glib::RefPtr<Gst::Element>& element)
: param_spec(param_spec),
  element(element),
  widget(nullptr)
{}

Property::~Property()
{
	delete widget;
}

QWidget* Property::get_widget()
{
	if (widget == nullptr)
		build_widget();

	return widget;
}

Property* Property::build_property(GParamSpec* param_spec,
		const Glib::RefPtr<Gst::Element>& element, const std::string& str_value)
{
	GValue value = { 0, };
	g_value_init (&value, param_spec->value_type);
	GType value_type = G_VALUE_TYPE(&value);

	if (GstUtils::is_numeric_type(value_type))
		return build_numeric_property(param_spec, element, value_type, str_value);
	/*else if( G_IS_PARAM_SPEC_ENUM (param_spec))
		return new PropertyEnum(param_spec, element);*/
	else
	{
		switch (value_type)
		{
		case G_TYPE_BOOLEAN:
			return new PropertyBoolean(param_spec, element,
					StringUtils::str_to_numeric<bool>(str_value));
		case G_TYPE_STRING:
			return new PropertyString(param_spec, element, str_value);/*
		case GST_TYPE_CAPS:
			return new PropertyCaps(param_spec, element);*/
		}
	}
	return nullptr;
}

Property* Property::build_numeric_property(GParamSpec* param_spec,
		const Glib::RefPtr<Gst::Element>& element, GType type, const std::string& value)
{
#define NUM_CASE(g_type, type) case g_type: \
		return new PropertyNumeric<type>(param_spec, element, \
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
}
