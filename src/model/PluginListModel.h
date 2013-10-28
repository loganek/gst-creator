/*
 * PluginListModel.h
 *
 *  Created on: 7 paź 2013
 *      Author: Marcin Kolny <marcin.kolny[at]gmail.com>
 */

#ifndef PLUGINLISTMODEL_H_
#define PLUGINLISTMODEL_H_

#include "FactoryItem.h"
#include <gstreamermm.h>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <boost/optional.hpp>

class PluginListModel : public QAbstractItemModel
{
private:
	boost::optional<QString> user_filter;

	void setup_model_data();

	std::vector<FactoryItem*> items;
	FactoryItem* root_item;

	void add_plugin_to_model(const Glib::RefPtr<Gst::Plugin>& plugin);

	Q_OBJECT

public:
	explicit PluginListModel(QObject *parent = 0);
	~PluginListModel();

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column,
			const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	Qt::DropActions supportedDragActions() const;
	Qt::DropActions supportedDropActions() const;
};

#endif /* PLUGINLISTMODEL_H_ */
