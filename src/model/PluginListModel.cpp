/*
 * PluginListModel.cpp
 *
 *  Created on: 7 paź 2013
 *      Author: Marcin Kolny <marcin.kolny[at]gmail.com>
 */

#include "PluginListModel.h"
#include "ElementFactoryItem.h"
#include "TypeFindFactoryItem.h"
#include "PluginItem.h"
#include <glibmm.h>
#include <regex>

using namespace Gst;
using Glib::RefPtr;

PluginListModel::PluginListModel(QObject *parent)
: QAbstractItemModel(parent)
{
	root_item = new FactoryItem();
	setup_model_data();
}

PluginListModel::~PluginListModel()
{
	delete root_item;
}

int PluginListModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}

QVariant PluginListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	FactoryItem *item = static_cast<FactoryItem*>(index.internalPointer());

	return QVariant(
			index.column() == 0 ?
					item->get_name().c_str() :
					item->get_desc().c_str());
}

Qt::ItemFlags PluginListModel::flags(const QModelIndex &index) const
{
	FactoryItem *item = static_cast<FactoryItem*>(index.internalPointer());

	if (dynamic_cast<PluginItem*>(item) != 0 )
		return 0;

	if (!index.isValid())
		return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

	return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
}

QVariant PluginListModel::headerData(int section, Qt::Orientation orientation,
		int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return QVariant(FactoryItem::get_header(section).c_str());

	return QVariant();
}

QModelIndex PluginListModel::index(int row, int column, const QModelIndex &parent)
const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	FactoryItem* parent_item;

	if (!parent.isValid())
		parent_item = root_item;
	else
		parent_item = static_cast<FactoryItem*>(parent.internalPointer());

	FactoryItem *childItem = parent_item->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex PluginListModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	FactoryItem *child_item = static_cast<FactoryItem*>(index.internalPointer());
	FactoryItem *parent_item = child_item->parent();

	if (parent_item == root_item)
		return QModelIndex();

	return createIndex(parent_item->row(), 0, parent_item);
}

int PluginListModel::rowCount(const QModelIndex &parent) const
{
	FactoryItem *parent_item;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parent_item = root_item;
	else
		parent_item = static_cast<FactoryItem*>(parent.internalPointer());

	return parent_item->child_count();
}

void PluginListModel::setup_model_data()
{
	RefPtr<Registry> registry = Registry::get();

	delete root_item;
	root_item = new FactoryItem();

	Glib::ListHandle<RefPtr<Plugin>> plugins = registry->get_plugin_list();

	for (auto plugin : plugins)
	{
		add_plugin_to_model(plugin);
	}
}

void PluginListModel::add_plugin_to_model(const RefPtr<Plugin>& plugin)
{
	if (GST_OBJECT_FLAG_IS_SET (plugin->gobj(), GST_PLUGIN_FLAG_BLACKLISTED))
	{
		return;
	}

	RefPtr<Registry> registry = Registry::get();
	PluginItem* plugin_item = new PluginItem(plugin, root_item);

	root_item->append_child(plugin_item);

	Glib::ListHandle<RefPtr<PluginFeature>> features = registry->get_feature_list(plugin->get_name());

	for (auto feature : features)
	{
		if (!feature)
			continue;

		if (GST_IS_ELEMENT_FACTORY (feature->gobj()))
		{
			RefPtr<ElementFactory> factory = factory.cast_static(feature);
			factory->reference();
			plugin_item->append_child(new ElementFactoryItem(factory, plugin_item));
		}
		else if (GST_IS_TYPE_FIND_FACTORY (feature->gobj()))
		{
			RefPtr<TypeFindFactory> factory = factory.cast_static(feature);

			plugin_item->append_child(new TypeFindFactoryItem(factory, plugin_item));
		}
	}
}

Qt::DropActions PluginListModel::supportedDragActions() const
{
 return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions PluginListModel::supportedDropActions() const
{
 return Qt::CopyAction | Qt::MoveAction;
}
