/*
 * ObjectInspectorFilter.h
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef OBJECTINSPECTORFILTER_H_
#define OBJECTINSPECTORFILTER_H_

#include <QSortFilterProxyModel>

class ObjectInspectorFilter : public QSortFilterProxyModel
{
private:
	bool should_be_displayed(const QModelIndex index) const;

public:
	ObjectInspectorFilter(QObject *parent = NULL);
	bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
};

#endif /* OBJECTINSPECTORFILTER_H_ */
