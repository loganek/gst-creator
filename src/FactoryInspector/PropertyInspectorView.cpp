/*
 * PropertyInspectorView.cpp
 *
 *  Created on: 30 pa? 2013
 *      Author: Marcin Kolny
 */

#include "PropertyInspectorView.h"
#include "FactoryInspectorView.h"

using namespace Gst;
using Glib::RefPtr;

#define NUMERIC_PROPERTY(Type, TYPE, type, preety_type) \
		{\
	GParamSpec##Type *pvalue = G_PARAM_SPEC_##TYPE (param);\
	item->addChild(new QTreeWidgetItem({"Type", preety_type})); \
	item->addChild(new QTreeWidgetItem({"Default value", std::to_string(g_value_get_##type(&value)).c_str()})); \
	item->addChild(new QTreeWidgetItem({"Range", (std::to_string(pvalue->minimum) + " - " + std::to_string(pvalue->maximum)).c_str()})); \
	break; \
		}

PropertyInspectorView::PropertyInspectorView()
: tree(new QTreeWidget())
{
	tree->setColumnCount(2);
	tree->setHeaderLabels({"Property name", "Description"});
}

QWidget* PropertyInspectorView::get_property_inspector_view(const RefPtr<ElementFactory>& factory)
{
	GParamSpec **property_specs;
	guint property_count;
	RefPtr<Element> element = ElementFactory::create_element(factory->get_name());

	property_specs = g_object_class_list_properties(G_OBJECT_GET_CLASS(element->gobj()), &property_count);

	for (int i = 0; i < property_count; i++)
	{
		GValue value = { 0, };
		GParamSpec* param = property_specs[i];
		g_value_init (&value, param->value_type);

		QTreeWidgetItem* item = new QTreeWidgetItem({g_param_spec_get_name(param),
			g_param_spec_get_blurb(param)});

		if (param->flags & G_PARAM_READABLE)
			g_object_get_property(G_OBJECT (element->gobj()), param->name, &value);
		else
			g_param_value_set_default(param, &value);

		Glib::ustring flags = get_flags_string(param->flags);

		item->addChild(new QTreeWidgetItem({"Flags", flags.c_str()}));

		GType value_type = G_VALUE_TYPE(&value);

		if (is_numeric_property(value_type))
			generate_numeric_property(item, value, param);

		else if (value_type == G_TYPE_STRING)
		{
			const char *string_val = g_value_get_string (&value);
			item->addChild(new QTreeWidgetItem({"Type", "String"}));
			item->addChild(new QTreeWidgetItem({"Default value", (string_val == nullptr ? "NULL" : string_val)}));
		}
		else if (value_type == G_TYPE_BOOLEAN)
		{
			gboolean bool_val = g_value_get_boolean (&value);
			item->addChild(new QTreeWidgetItem({"Type", "Boolean"}));
			item->addChild(new QTreeWidgetItem({"Default value", bool_val ? "true" : "false"}));
		}
		else
			generate_various_property(item, value, param);

		tree->addTopLevelItem(item);
	}

	return tree;
}

Glib::ustring PropertyInspectorView::get_flags_string(GParamFlags param_flags)
{
	Glib::ustring flags;

	if (param_flags & G_PARAM_READABLE)
		flags += "(readable) ";
	if (param_flags & G_PARAM_WRITABLE)
		flags += "(writable) ";
	if (param_flags & GST_PARAM_CONTROLLABLE)
		flags += "(controllable) ";
	if (param_flags & GST_PARAM_MUTABLE_PLAYING)
		flags += "(changeable in NULL, READY, PAUSED or PLAYING state) ";
	else if (param_flags & GST_PARAM_MUTABLE_PAUSED)
		flags += "(changeable only in NULL, READY or PAUSED state) ";
	else if (param_flags & GST_PARAM_MUTABLE_READY)
		flags += "(changeable only in NULL or READY state) ";

	if (flags.empty())
		flags = "EMPTY";

	return flags;
}

bool PropertyInspectorView::is_numeric_property(GType type)
{
	return (type & G_TYPE_ULONG) || (type & G_TYPE_LONG)
			|| (type & G_TYPE_UINT) || (type & G_TYPE_INT)
			|| (type & G_TYPE_UINT64) || (type & G_TYPE_INT64)
			|| (type & G_TYPE_FLOAT) || (type & G_TYPE_DOUBLE);
}

void PropertyInspectorView::generate_numeric_property(QTreeWidgetItem* item, GValue value, GParamSpec* param)
{
	GType value_type = G_VALUE_TYPE(&value);

	switch (value_type)
	{
	case G_TYPE_ULONG:
		NUMERIC_PROPERTY(ULong, ULONG, ulong, "Unsigned Long")
	case G_TYPE_LONG:
		NUMERIC_PROPERTY(Long, LONG, long, "Long")
	case G_TYPE_UINT:
		NUMERIC_PROPERTY(UInt, UINT, uint, "Unsigned Integer")
	case G_TYPE_INT:
		NUMERIC_PROPERTY(Int, INT, int, "Integer")
	case G_TYPE_UINT64:
		NUMERIC_PROPERTY(UInt64, UINT64, uint64, "Unsigned Integer64")
	case G_TYPE_INT64:
		NUMERIC_PROPERTY(Int64, INT64, int64, "Integer64")
	case G_TYPE_FLOAT:
		NUMERIC_PROPERTY(Float, FLOAT, float, "Float")
	case G_TYPE_DOUBLE:
		NUMERIC_PROPERTY(Double, DOUBLE, double, "Double")
	}
}


static gchar *
flags_to_string (GFlagsValue * vals, guint flags)
{
	Glib::ustring str;

	int i;
	for (i = 0; vals[i].value_name != nullptr; ++i)
		if (vals[i].value == flags)
			return g_strdup(vals[i].value_nick);

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

	return g_strdup(str.empty() ? "none" : str.c_str());
}

void PropertyInspectorView::generate_various_property(QTreeWidgetItem* item, GValue value, GParamSpec* param)
{
	if (param->value_type == GST_TYPE_CAPS)
	{
		RefPtr<Caps> caps = Glib::wrap(const_cast<GstCaps*>(gst_value_get_caps(&value)));
		item->addChild(FactoryInspectorView::get_caps_view(caps));
	}
	else if (G_IS_PARAM_SPEC_ENUM (param))
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
	else if (G_IS_PARAM_SPEC_FLAGS (param))
	{
		GParamSpecFlags *pflags = G_PARAM_SPEC_FLAGS (param);
		GFlagsValue *vals;

		vals = pflags->flags_class->values;

		char* cur = flags_to_string(vals, g_value_get_flags (&value));

		item->addChild(new QTreeWidgetItem({"Type", QString("Flags ") + g_type_name (G_VALUE_TYPE (&value))}));
		item->addChild(new QTreeWidgetItem({"Default", cur}));

		std::string values;
		while (vals[0].value_name)
		{
			values += std::to_string(vals->value);
			values += std::string(": ") + vals->value_nick;
			values += std::string(" - ") + vals->value_name;
			++vals;
		}

		g_free (cur);
	}
	else if (G_IS_PARAM_SPEC_OBJECT (param))
		item->addChild(new QTreeWidgetItem({"Type", QString("Object of type ") +
				g_type_name (param->value_type)}));
	else if (G_IS_PARAM_SPEC_BOXED (param))
		item->addChild(new QTreeWidgetItem({"Type", QString("Boxed pointer of type") +
		g_type_name (param->value_type)}));
	else if (G_IS_PARAM_SPEC_POINTER (param))
	{
		if (param->value_type != G_TYPE_POINTER)
			item->addChild(new QTreeWidgetItem({"Type", QString("Pointer of type") +
			g_type_name (param->value_type)}));
		else
			item->addChild(new QTreeWidgetItem({"Type", "Pointer"}));
	}

	else if (param->value_type == G_TYPE_ARRAY)
	{
		GParamSpecValueArray *pvarray = G_PARAM_SPEC_VALUE_ARRAY (param);

		if (pvarray->element_spec)
			item->addChild(new QTreeWidgetItem({"Type", QString("Array of GValues of type ") +
			g_type_name (pvarray->element_spec->value_type)}));
		else
			item->addChild(new QTreeWidgetItem({"Type", "Array of GValues"}));
	}
	else if (GST_IS_PARAM_SPEC_FRACTION (param))
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
	else
		item->addChild(new QTreeWidgetItem({"Type", QString("Unknow type ") + g_type_name(param->value_type)}));
}
