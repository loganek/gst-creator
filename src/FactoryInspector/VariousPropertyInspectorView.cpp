/*
 * VariousPropertyInspectorView.cpp
 *
 *  Created on: 30 pa? 2013
 *      Author: Marcin Kolny
 */

#include "VariousPropertyInspectorView.h"
#include "FactoryInspectorView.h"

using namespace Gst;
using Glib::RefPtr;

VariousPropertyInspectorView::VariousPropertyInspectorView(QTreeWidgetItem* item, GValue value, GParamSpec* param)
: item(item),
  value(value),
  param(param)
{}

Glib::ustring VariousPropertyInspectorView::flags_to_string(GFlagsValue * vals, guint flags)
{
	Glib::ustring str;

	int i;
	for (i = 0; vals[i].value_name != nullptr; ++i)
		if (vals[i].value == flags)
			return Glib::ustring(vals[i].value_nick);

	while (i-- > 0)
	{
		if (vals[i].value != 0 && (flags & vals[i].value) == vals[i].value)
		{
			if (!str.empty())
				str += '+';
			str += vals[i].value_nick;
			flags -= vals[i].value;
			if (flags == 0)
				break;
		}
	}

	return str.empty() ? "none" : str;
}

void VariousPropertyInspectorView::generate_various_property()
{
	if (param->value_type == GST_TYPE_CAPS)
	{
		RefPtr<Caps> caps = Glib::wrap(const_cast<GstCaps*>(gst_value_get_caps(&value)));
		item->addChild(FactoryInspectorView::get_caps_view(caps));
	}
	else if (G_IS_PARAM_SPEC_ENUM (param))
		param_spec_enum();
	else if (G_IS_PARAM_SPEC_FLAGS (param))
		param_spec_flags();
	else if (G_IS_PARAM_SPEC_OBJECT (param))
		item->addChild(new QTreeWidgetItem({"Type", QString("Object of type ") +
		g_type_name (param->value_type)}));
	else if (G_IS_PARAM_SPEC_BOXED (param))
		item->addChild(new QTreeWidgetItem({"Type", QString("Boxed pointer of type") +
		g_type_name (param->value_type)}));
	else if (G_IS_PARAM_SPEC_POINTER (param))
		param_spec_pointer();
	else if (param->value_type == G_TYPE_ARRAY)
		param_array();
	else if (GST_IS_PARAM_SPEC_FRACTION (param))
		param_spec_fraction();
	else
		item->addChild(new QTreeWidgetItem({"Type", QString("Unknow type ") + g_type_name(param->value_type)}));
}

void VariousPropertyInspectorView::param_spec_flags()
{
	GParamSpecFlags *pflags = G_PARAM_SPEC_FLAGS (param);
	GFlagsValue *vals;

	vals = pflags->flags_class->values;

	Glib::ustring flags = flags_to_string(vals, g_value_get_flags (&value));

	item->addChild(new QTreeWidgetItem({"Type", QString("Flags ") + g_type_name (G_VALUE_TYPE (&value))}));
	item->addChild(new QTreeWidgetItem({"Default", flags.c_str()}));

	std::string values;
	while (vals[0].value_name)
	{
		values += std::to_string(vals->value);
		values += std::string(": ") + vals->value_nick;
		values += std::string(" - ") + vals->value_name;
		values += "\n";
		++vals;
	}

	item->addChild(new QTreeWidgetItem({"Values", values.c_str()}));
}

void VariousPropertyInspectorView::param_array()
{
	GParamSpecValueArray *pvarray = G_PARAM_SPEC_VALUE_ARRAY (param);

	if (pvarray->element_spec)
		item->addChild(new QTreeWidgetItem({"Type", QString("Array of GValues of type ") +
		g_type_name (pvarray->element_spec->value_type)}));
	else
		item->addChild(new QTreeWidgetItem({"Type", "Array of GValues"}));
}

void VariousPropertyInspectorView::param_spec_fraction()
{
	GstParamSpecFraction *pfraction = GST_PARAM_SPEC_FRACTION (param);

	item->addChild(new QTreeWidgetItem({"Type", "Fraction"}));
	item->addChild(new QTreeWidgetItem({"Default value",
		QString(std::to_string(gst_value_get_fraction_numerator (&value)).c_str()) + "/" +
		std::to_string(gst_value_get_fraction_denominator (&value)).c_str()}));
	item->addChild(new QTreeWidgetItem({"Range",
		QString(std::to_string(pfraction->min_num).c_str()) + "/" + std::to_string(pfraction->min_den).c_str() + " - " +
		std::to_string(pfraction->max_num).c_str() + "/" + std::to_string(pfraction->max_den).c_str()}));
}

void VariousPropertyInspectorView::param_spec_pointer()
{
	if (param->value_type != G_TYPE_POINTER)
		item->addChild(new QTreeWidgetItem({"Type", QString("Pointer of type") +
		g_type_name (param->value_type)}));
	else
		item->addChild(new QTreeWidgetItem({"Type", "Pointer"}));
}

void VariousPropertyInspectorView::param_spec_enum()
{
	GEnumValue *values;
	guint j = 0;
	gint enum_value;
	const gchar *value_nick = "";

	values = G_ENUM_CLASS (g_type_class_ref(param->value_type))->values;
	enum_value = g_value_get_enum (&value);

	item->addChild(new QTreeWidgetItem({"Type", "Enum"}));
	item->addChild(new QTreeWidgetItem({"Default", std::to_string(enum_value).c_str()}));

	QString enum_values;
	while (values[j].value_name)
	{
		enum_values += QString("(") + std::to_string(values[j].value).c_str() + ") " + values[j].value_nick + " - " + values[j].value_name + "\n";
		j++;
	}
	item->addChild(new QTreeWidgetItem({"Enum values", enum_values}));
}
