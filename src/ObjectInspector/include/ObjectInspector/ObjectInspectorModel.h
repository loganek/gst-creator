/*
 * ObjectInspectorModel.h
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef OBJECTINSPECTORMODEL_H_
#define OBJECTINSPECTORMODEL_H_


#include "ObjectInspectorItem.h"
#include <gstreamermm.h>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class ObjectInspectorModel : public QAbstractItemModel
{
private:
	void setup_model_data();

	std::vector<ObjectInspectorItem*> items;
	ObjectInspectorItem* root_item;

	void add_plugin_to_model(const Glib::RefPtr<Gst::Plugin>& plugin);

	Q_OBJECT

public:
	explicit ObjectInspectorModel(QObject *parent = 0);
	~ObjectInspectorModel();

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column,
			const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
};


#endif /* OBJECTINSPECTORMODEL_H_ */
