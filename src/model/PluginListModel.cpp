/*
 * PluginListModel.cpp
 *
 *  Created on: 7 paź 2013
 *      Author: loganek
 */

#include "PluginListModel.h"
#include <glibmm.h>

using namespace Gst;
using Glib::RefPtr;

PluginListModel::PluginListModel(QObject *parent)
: QAbstractItemModel(parent)
{
	root_item = new ElementFactoryItem((RefPtr<ElementFactory>)0);
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

	ElementFactoryItem *item = static_cast<ElementFactoryItem*>(index.internalPointer());

	return QVariant("Test");
}

Qt::ItemFlags PluginListModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return QAbstractItemModel::flags(index);
}

QVariant PluginListModel::headerData(int section, Qt::Orientation orientation,
		int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		//return rootItem->data(section);
		return QVariant("Drugi test");

	return QVariant();
}

QModelIndex PluginListModel::index(int row, int column, const QModelIndex &parent)
const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	ElementFactoryItem* parent_item;

	if (!parent.isValid())
		parent_item = root_item;
	else
		parent_item = static_cast<ElementFactoryItem*>(parent.internalPointer());

	ElementFactoryItem *childItem = parent_item->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex PluginListModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	ElementFactoryItem *child_item = static_cast<ElementFactoryItem*>(index.internalPointer());
	ElementFactoryItem *parent_item = child_item->parent();

	if (parent_item == root_item)
		return QModelIndex();

	return createIndex(parent_item->row(), 0, parent_item);
}

int PluginListModel::rowCount(const QModelIndex &parent) const
{
	ElementFactoryItem *parent_item;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parent_item = root_item;
	else
		parent_item = static_cast<ElementFactoryItem*>(parent.internalPointer());

	return parent_item->child_count();
}

#include <gst/gst.h>
void PluginListModel::setup_model_data()
{
	RefPtr<Registry> registry = Registry::get();

	Glib::ListHandle<RefPtr<Plugin>> plugins = registry->get_plugin_list();
	int blacklist_count = 0,
			feature_count = 0;

	for (auto plugin : plugins)
	{
		if (GST_OBJECT_FLAG_IS_SET (plugin->gobj(), GST_PLUGIN_FLAG_BLACKLISTED))
		{
			blacklist_count++;
			continue;
		}

		Glib::ListHandle<RefPtr<PluginFeature>> features = registry->get_feature_list(plugin->get_name());

		for (auto feature : features)
		{
			if (!feature)
				continue;

			feature_count++;

			if (GST_IS_ELEMENT_FACTORY (feature->gobj()))
			{
				RefPtr<ElementFactory> factory = factory.cast_static(feature);

				g_print ("%s:  %s: %s\n", plugin->get_name().c_str(),
						factory->get_name().c_str(),
						factory->get_metadata(GST_ELEMENT_METADATA_LONGNAME).c_str());
			}
			else if (GST_IS_TYPE_FIND_FACTORY (feature->gobj()))
			{
				RefPtr<TypeFindFactory> factory = factory.cast_static(feature);

				g_print ("%s: %s: ", plugin->get_name().c_str(),
						feature->get_name().c_str());

				std::vector<Glib::ustring> extensions = factory->get_extensions();

				if (!extensions.empty())
				{
					guint i = 0;

					for (auto extension : extensions)
					{
						g_print ("%s%s", i > 0 ? ", " : "", extension.c_str());
						i++;
					}
					g_print ("\n");
				}
				else
				{
					g_print ("no extensions\n");
				}
			}
			else
			{
				g_print ("%s:  %s (%s)\n", plugin->get_name().c_str(),
						feature->get_name().c_str(), feature->get_name().c_str());
			}
		}
	}

	QList<ElementFactoryItem*> parents;
	QList<int> indentations;
	parents << root_item;
	indentations << 0;

	int number = 0;

	for (int i = 0; i < 10; i++)
	{
		QList<QVariant> columnData;


		parents.last()->append_child(new ElementFactoryItem((RefPtr<ElementFactory>)0, parents.last()));
	}
}

