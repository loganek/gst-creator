/*
 * ObjectInspectorModel.cpp
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#include "ObjectInspectorModel.h"
#include "PluginItem.h"
#include <glibmm.h>
#include <regex>

using namespace Gst;
using Glib::RefPtr;

ObjectInspectorModel::ObjectInspectorModel(QObject *parent)
: QAbstractItemModel(parent),
  root_item(nullptr)
{
	setup_model_data();
}

ObjectInspectorModel::~ObjectInspectorModel()
{
	delete root_item;
}

int ObjectInspectorModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant ObjectInspectorModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	ObjectInspectorItem *item = static_cast<ObjectInspectorItem*>(index.internalPointer());

	return QVariant(item->get_name().c_str());
}

Qt::ItemFlags ObjectInspectorModel::flags(const QModelIndex &index) const
{
	ObjectInspectorItem *item = static_cast<ObjectInspectorItem*>(index.internalPointer());

	if (dynamic_cast<PluginItem*>(item) != 0 )
		return 0;

	if (!index.isValid())
		return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

	return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
}

QVariant ObjectInspectorModel::headerData(int section, Qt::Orientation orientation,
		int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return QVariant("Factory name");

	return QVariant();
}

QModelIndex ObjectInspectorModel::index(int row, int column, const QModelIndex &parent)
const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	ObjectInspectorItem* parent_item;

	if (!parent.isValid())
		parent_item = root_item;
	else
		parent_item = static_cast<ObjectInspectorItem*>(parent.internalPointer());

	ObjectInspectorItem *childItem = parent_item->child(row);

	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex ObjectInspectorModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	ObjectInspectorItem *child_item = static_cast<ObjectInspectorItem*>(index.internalPointer());
	ObjectInspectorItem *parent_item = child_item->parent();

	if (parent_item == root_item)
		return QModelIndex();

	return createIndex(parent_item->row(), 0, parent_item);
}

int ObjectInspectorModel::rowCount(const QModelIndex &parent) const
{
	ObjectInspectorItem *parent_item;

	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parent_item = root_item;
	else
		parent_item = static_cast<ObjectInspectorItem*>(parent.internalPointer());

	return parent_item->child_count();
}

void ObjectInspectorModel::setup_model_data()
{
	RefPtr<Registry> registry = Registry::get();

	delete root_item;
	root_item = new ObjectInspectorItem("");

	for (RefPtr<Plugin> plugin : registry->get_plugin_list())
		add_plugin_to_model(plugin);
}

void ObjectInspectorModel::add_plugin_to_model(const RefPtr<Plugin>& plugin)
{
	if (GST_OBJECT_FLAG_IS_SET (plugin->gobj(), GST_PLUGIN_FLAG_BLACKLISTED))
		return;

	RefPtr<Registry> registry = Registry::get();
	PluginItem* plugin_item = new PluginItem(plugin, root_item);

	root_item->append_child(plugin_item);

	Glib::ListHandle<RefPtr<PluginFeature>> features = registry->get_feature_list(plugin->get_name());

	for (auto feature : features)
	{
		if (!feature)
			continue;

		FactoryType type = GST_IS_ELEMENT_FACTORY (feature->gobj()) ?
				FactoryType::ELEMENT_FACTORY :
				FactoryType::TYPEFIND_FACTORY;

		ObjectInspectorItem* item = new ObjectInspectorItem(feature->get_name().c_str(), type, plugin_item);

		plugin_item->append_child(item);
	}
}
