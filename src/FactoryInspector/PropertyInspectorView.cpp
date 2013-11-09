/*
 * PropertyInspectorView.cpp
 *
 *  Created on: 30 pa? 2013
 *      Author: Marcin Kolny
 */

#include "PropertyInspectorView.h"
#include "VariousPropertyInspectorView.h"
#include "utils/GstUtils.h"

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
		tree->addTopLevelItem(generate_property_item(property_specs[i], element->gobj()));
	}

	return tree;
}

QTreeWidgetItem* PropertyInspectorView::generate_property_item(GParamSpec* param, GstElement* element)
{
	GValue value = { 0, };
	g_value_init (&value, param->value_type);

	QTreeWidgetItem* item = new QTreeWidgetItem({g_param_spec_get_name(param),
		g_param_spec_get_blurb(param)});

	if (param->flags & G_PARAM_READABLE)
		g_object_get_property(G_OBJECT (element), param->name, &value);
	else
		g_param_value_set_default(param, &value);

	Glib::ustring flags = get_flags_string(param->flags);
	item->addChild(new QTreeWidgetItem({"Flags", flags.c_str()}));

	GType value_type = G_VALUE_TYPE(&value);

	if (GstUtils::is_numeric_type(value_type))
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
	{
		VariousPropertyInspectorView various_property(item, value, param);
		various_property.generate_various_property();
	}

	return item;
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

