/*
 * FactoryTreeProxyFilter.cpp
 *
 *  Created on: 13 pa? 2013
 *      Author: Marcin Kolny
 */

#include "FactoryTreeProxyFilter.h"

FactoryTreeProxyFilter::FactoryTreeProxyFilter(QObject *parent):
		QSortFilterProxyModel(parent)
{
}

bool FactoryTreeProxyFilter::filterAcceptsRow(int sourceRow,
		const QModelIndex &sourceParent) const
{
	QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

	return should_be_displayed(index);
}

bool FactoryTreeProxyFilter::should_be_displayed(const QModelIndex index) const
{
	bool result = false;

	if ( sourceModel()->rowCount(index) > 0 )
	{
		for (int i = 0; i < sourceModel()->rowCount(index); i++)
		{
			for (int j = 0; j < sourceModel()->columnCount(); j++)
			{
				QModelIndex childIndex = sourceModel()->index(i, j, index);
				if (!childIndex.isValid())
					break;

				if (should_be_displayed(childIndex))
					return true;
			}
		}
	}

	for (int j = 0 ; j < sourceModel()->columnCount(); j++)
	{
		QModelIndex useIndex = sourceModel()->index(index.row(), j, index.parent());
		QString type = sourceModel()->data(useIndex, Qt::DisplayRole).toString();

		if(type.contains(filterRegExp()))
			return true;
	}

	return false;
}

