/*
 * FactoryTreeProxyFilter.h
 *
 *  Created on: 13 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef FACTORYTREEPROXYFILTER_H_
#define FACTORYTREEPROXYFILTER_H_

#include <QSortFilterProxyModel>

class FactoryTreeProxyFilter : public QSortFilterProxyModel
{
public:
	FactoryTreeProxyFilter(QObject *parent = NULL);
	bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
private :
	bool should_be_displayed(const QModelIndex index) const;
};


#endif /* FACTORYTREEPROXYFILTER_H_ */
