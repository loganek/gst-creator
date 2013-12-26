/*
 * FactoryInspectorView.cpp
 *
 *  Created on: 29 pa? 2013
 *      Author: Marcin Kolny
 */

#include "FactoryInspectorView.h"
#include "PropertyInspectorView.h"

using namespace Gst;
using Glib::RefPtr;

FactoryInspectorView::FactoryInspectorView(bool is_factory, const QString& name, QWidget* parent)
: QWidget(parent)
{
	setLayout(new QVBoxLayout());

	if (is_factory)
	{
		RefPtr<ElementFactory> factory = ElementFactory::find(name.toUtf8().constData());
		if (!factory)
			return;

		build_element_factory_view(factory);
		build_pad_templates_view(factory);
		build_pads_view(factory);
		build_properties_view(factory);
		build_plugin_view(factory->get_plugin());
	}
	else
	{
		RefPtr<Plugin> plugin = Plugin::load_by_name(name.toUtf8().constData());

		if (!plugin)
			return;

		build_plugin_view(plugin);
	}
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

	for (guint i = 0; i < caps->size(); i++)
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
		RefPtr<PadTemplate> tpl = iterator->get_pad_template();
		if (tpl)
			item->addChild(new QTreeWidgetItem({"Pad template", tpl->get_name().c_str()}));
	}

	layout()->addWidget(new QLabel("Pads:"));
	layout()->addWidget(tree);
}


void FactoryInspectorView::build_properties_view(const RefPtr<ElementFactory>& factory)
{
	PropertyInspectorView property_view;
	QWidget* properties = property_view.get_property_inspector_view(factory);

	layout()->addWidget(new QLabel("Properties:"));
	layout()->addWidget(properties);
}
