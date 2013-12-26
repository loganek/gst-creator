/*
 * ObjectInspectorFilter.cpp
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#include "ObjectInspectorFilter.h"

ObjectInspectorFilter::ObjectInspectorFilter(QObject *parent):
		QSortFilterProxyModel(parent)
{
}

bool ObjectInspectorFilter::filterAcceptsRow(int sourceRow,
		const QModelIndex &sourceParent) const
{
	QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

	return should_be_displayed(index);
}

bool ObjectInspectorFilter::should_be_displayed(const QModelIndex index) const
{
	if ( sourceModel()->rowCount(index) > 0 )
	{
		for (int i = 0; i < sourceModel()->rowCount(index); i++)
		{
			QModelIndex childIndex = sourceModel()->index(i, 0, index);

			if (!childIndex.isValid())
				break;
			if (should_be_displayed(childIndex))
				return true;
		}
	}

	QModelIndex useIndex = sourceModel()->index(index.row(), 0, index.parent());
	QString type = sourceModel()->data(useIndex, Qt::DisplayRole).toString();

	if(type.contains(filterRegExp()))
		return true;

	return false;
}


