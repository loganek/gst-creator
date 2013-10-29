/*
 * FactoryInspectorView.cpp
 *
 *  Created on: 29 pa? 2013
 *      Author: Marcin Kolny
 */

#include "FactoryInspectorView.h"

using namespace Gst;
using Glib::RefPtr;

FactoryInspectorView::FactoryInspectorView(const QString& factory_name, QWidget* parent)
: QWidget(parent)
{
	setLayout(new QVBoxLayout());

	RefPtr<ElementFactory> factory = ElementFactory::find(factory_name.toUtf8().constData());

	if (factory)
	{
		build_element_factory_view(factory);
		build_pad_templates_view(factory);
		build_pads_view(factory);
		build_properties_view(factory);
	}

	RefPtr<Plugin> plugin = factory->get_plugin();

	build_plugin_view(plugin);
}

FactoryInspectorView::~FactoryInspectorView()
{

}

void FactoryInspectorView::build_element_factory_view(const RefPtr<ElementFactory>& factory)
{
	std::vector<Glib::ustring> keys = factory->get_metadata_keys();
	QTreeWidget* tree = new QTreeWidget();
	tree->setColumnCount(2);
	tree->setHeaderLabels({"Metadata key", "Metadata value"});

	for (auto key : keys)
		tree->addTopLevelItem(new QTreeWidgetItem({key.c_str(), factory->get_metadata(key).c_str()}));

	layout()->addWidget(new QLabel("Factory Details:"));
	layout()->addWidget(tree);
}

void FactoryInspectorView::build_plugin_view(const RefPtr<Plugin>& plugin)
{
	QTreeWidget* tree = new QTreeWidget();
	tree->setColumnCount(2);
	tree->setHeaderLabels({"Item name", "Item value"});

	tree->addTopLevelItem(new QTreeWidgetItem({"Name", plugin->get_name().c_str()}));
	tree->addTopLevelItem(new QTreeWidgetItem({"Description", plugin->get_description().c_str()}));
	tree->addTopLevelItem(new QTreeWidgetItem({"Filename", plugin->get_filename().c_str()}));
	tree->addTopLevelItem(new QTreeWidgetItem({"Version", plugin->get_version().c_str()}));
	tree->addTopLevelItem(new QTreeWidgetItem({"License", plugin->get_license().c_str()}));
	tree->addTopLevelItem(new QTreeWidgetItem({"Source module", plugin->get_source().c_str()}));
	tree->addTopLevelItem(new QTreeWidgetItem({"Source release date", plugin->get_release_date_string().c_str()}));
	tree->addTopLevelItem(new QTreeWidgetItem({"Binary package", plugin->get_package().c_str()}));
	tree->addTopLevelItem(new QTreeWidgetItem({"Origin URL", plugin->get_origin().c_str()}));

	layout()->addWidget(new QLabel("Plugin Details:"));
	layout()->addWidget(tree);

}

const char* FactoryInspectorView::pad_presence_to_string(PadPresence presence)
{
	switch (presence)
	{
	case PAD_ALWAYS:
		return "always";
	case PAD_REQUEST:
		return "on request";
	case PAD_SOMETIMES:
		return "sometimes";
	default:
		return "unknow presence";
	}
}

const char* FactoryInspectorView::pad_direction_to_string(PadDirection direction)
{
	switch (direction)
	{
	case PAD_SINK:
		return "SINK";
	case PAD_SRC:
		return "SRC";
	case PAD_UNKNOWN:
		return "UNKNOWN";
	default:
		return "unknow pad direction";
	}
}

void FactoryInspectorView::build_pad_templates_view(const RefPtr<ElementFactory>& factory)
{
	std::vector<StaticPadTemplate> pad_templates = factory->get_static_pad_templates();
	QTreeWidget* tree = new QTreeWidget();
	tree->setColumnCount(2);
	tree->setHeaderLabels({"Template", "Description"});

	for (auto pad_template : pad_templates)
	{
		RefPtr<PadTemplate> tpl = pad_template.get();

		QTreeWidgetItem* item = new QTreeWidgetItem({tpl->get_name().c_str(), pad_direction_to_string(tpl->get_direction()) + QString(" template")});
		item->addChild(new QTreeWidgetItem({"Availability", pad_presence_to_string(tpl->get_presence())}));
		RefPtr<Caps> caps = tpl->get_caps();

		item->addChild(get_caps_view(caps));

		tree->addTopLevelItem(item);
	}

	layout()->addWidget(new QLabel("Pad Templates:"));
	layout()->addWidget(tree);
}

QTreeWidgetItem* FactoryInspectorView::get_caps_view(const RefPtr<Caps>& caps)
{
	Glib::ustring caps_str;

	bool finish = true;

	if (!caps || caps->empty())
		caps_str = "EMPTY";
	else if (caps->is_any())
		caps_str = "ANY";
	else finish = false;

	QTreeWidgetItem* caps_item = new QTreeWidgetItem({"Caps", caps_str.c_str()});

	if (finish)
		return caps_item;

	for (int i = 0; i < caps->size(); i++)
	{
		Structure structure = caps->get_structure(i);
		QTreeWidgetItem* format_item = new QTreeWidgetItem({structure.get_name().c_str()});
		caps_item->addChild(format_item);

		for (int j = 0; j < structure.size(); j++)
		{
			Glib::ustring field_name = structure.get_nth_field_name(j);
			Glib::ValueBase base;

			structure.get_field(field_name, base);
			format_item->addChild(new QTreeWidgetItem({field_name.c_str(), gst_value_serialize (base.gobj())}));
		}
	}

	return caps_item;
}

void FactoryInspectorView::build_pads_view(const RefPtr<ElementFactory>& factory)
{
	RefPtr<Element> element = ElementFactory::create_element(factory->get_name());

	QTreeWidget* tree = new QTreeWidget();
	tree->setColumnCount(2);
	tree->setHeaderLabels({"Pad", "Description"});
	auto iterator = element->iterate_pads();

	while (iterator.next())
	{
		QTreeWidgetItem* item = new QTreeWidgetItem({iterator->get_name().c_str(),
			pad_direction_to_string(iterator->get_direction())});
		tree->addTopLevelItem(item);
		item->addChild(new QTreeWidgetItem({"Pad template", iterator->get_pad_template()->get_name().c_str()}));
	}

	layout()->addWidget(new QLabel("Pads:"));
	layout()->addWidget(tree);
}

#define NUMERIC_PROPERTY(Type, TYPE, type, preety_type) \
{\
	GParamSpec##Type *pvalue = G_PARAM_SPEC_##TYPE (param);\
	item->addChild(new QTreeWidgetItem({"Type", preety_type})); \
	item->addChild(new QTreeWidgetItem({"Default value", std::to_string(g_value_get_##type(&value)).c_str()})); \
	item->addChild(new QTreeWidgetItem({"Range", (std::to_string(pvalue->minimum) + " - " + std::to_string(pvalue->maximum)).c_str()})); \
	break; \
}

void FactoryInspectorView::build_properties_view(const RefPtr<ElementFactory>& factory)
{
	QTreeWidget* tree = new QTreeWidget();
	tree->setColumnCount(2);
	tree->setHeaderLabels({"Property name", "Description"});
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

		Glib::ustring flags;
		if (param->flags & G_PARAM_READABLE)
		{
			g_object_get_property (G_OBJECT (element->gobj()), param->name, &value);
			flags += "readable ";
		}
		else
			g_param_value_set_default (param, &value);

		if (param->flags & G_PARAM_WRITABLE)
			flags += "(writable) ";
		if (param->flags & GST_PARAM_CONTROLLABLE)
			flags += "(controllable) ";
		if (param->flags & GST_PARAM_MUTABLE_PLAYING)
			flags += "(changeable in NULL, READY, PAUSED or PLAYING state) ";
		else if (param->flags & GST_PARAM_MUTABLE_PAUSED)
			flags += "(changeable only in NULL, READY or PAUSED state) ";
		else if (param->flags & GST_PARAM_MUTABLE_READY)
			flags += "(changeable only in NULL or READY state) ";
		if (flags.empty())
			flags = "EMPTY";

		item->addChild(new QTreeWidgetItem({"Flags", flags.c_str()}));

		switch (G_VALUE_TYPE (&value))
		{
		case G_TYPE_STRING:
		{
			const char *string_val = g_value_get_string (&value);
			item->addChild(new QTreeWidgetItem({"Type", "String"}));
			item->addChild(new QTreeWidgetItem({"Default value", (string_val == nullptr ? "NULL" : string_val)}));
			break;
		}
		case G_TYPE_BOOLEAN:
		{
			gboolean bool_val = g_value_get_boolean (&value);
			item->addChild(new QTreeWidgetItem({"Type", "Boolean"}));
			item->addChild(new QTreeWidgetItem({"Default value", bool_val ? "true" : "false"}));
			break;
		}
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
		default:
		{
			if (param->value_type == GST_TYPE_CAPS)
			{
				RefPtr<Caps> caps = Glib::wrap(const_cast<GstCaps*>(gst_value_get_caps(&value)));
				item->addChild(get_caps_view(caps));
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
		}
		// TODO:G_IS_PARAM_SPEC_FLAGS etc. implemented
		// TODO: move property view to another class
		}

		tree->addTopLevelItem(item);
	}

	layout()->addWidget(new QLabel("Properties:"));
	layout()->addWidget(tree);
}
