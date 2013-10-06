/*
 * PluginListModel.cpp
 *
 *  Created on: 7 paź 2013
 *      Author: loganek
 */

#include "PluginListModel.h"

PluginListModel::PluginListModel(QObject *parent)
: QAbstractItemModel(parent)
{
	root_item = new ElementItem(Glib::RefPtr<Gst::Element>());
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

	ElementItem *item = static_cast<ElementItem*>(index.internalPointer());

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

	ElementItem* parent_item;

	if (!parent.isValid())
		parent_item = root_item;
	else
		parent_item = static_cast<ElementItem*>(parent.internalPointer());

	ElementItem *childItem = parent_item->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex PluginListModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	ElementItem *child_item = static_cast<ElementItem*>(index.internalPointer());
	ElementItem *parent_item = child_item->parent();

	if (parent_item == root_item)
		return QModelIndex();

	return createIndex(parent_item->row(), 0, parent_item);
}

int PluginListModel::rowCount(const QModelIndex &parent) const
{
	ElementItem *parent_item;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parent_item = root_item;
	else
		parent_item = static_cast<ElementItem*>(parent.internalPointer());

	return parent_item->child_count();
}

void PluginListModel::setup_model_data()
{
	QList<ElementItem*> parents;
	QList<int> indentations;
	parents << root_item;
	indentations << 0;

	int number = 0;

	for (int i = 0; i < 10; i++)
	{
		QList<QVariant> columnData;

		for (int column = 0; column < 2; ++column)
			columnData << "testowy";

		parents.last()->append_child(new ElementItem(Glib::RefPtr<Gst::Element>(), parents.last()));
	}
}

