/*
 * PluginListModel.h
 *
 *  Created on: 7 paź 2013
 *      Author: loganek
 */

#ifndef PLUGINLISTMODEL_H_
#define PLUGINLISTMODEL_H_

#include "ElementItem.h"
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class PluginListModel : public QAbstractItemModel
{
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

private:
	void setup_model_data();

	ElementItem *root_item;
};

#endif /* PLUGINLISTMODEL_H_ */
