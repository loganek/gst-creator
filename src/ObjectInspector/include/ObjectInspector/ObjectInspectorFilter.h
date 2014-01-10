/*
 * ObjectInspectorFilter.h
 *
 * Plik zawiera definicję klasy ObjectInspectorFilter
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef OBJECTINSPECTORFILTER_H_
#define OBJECTINSPECTORFILTER_H_

#include <QSortFilterProxyModel>

/**
 * Klasa umożliwiająca filtrowanie listy wtyczek i fabryk elementów
 */
class ObjectInspectorFilter : public QSortFilterProxyModel
{
private:
	/**
	 * Określa, czy element powinien być wyświetlony
	 *
	 * @param index indeks elementu
	 *
	 * @returns true, jeśli element ma zostać wyświetlony
	 */
	bool should_be_displayed(const QModelIndex index) const;

public:
	/**
	 * Konstruktor
	 *
	 * @param parent rodzic okna dialogowego
	 */
	ObjectInspectorFilter(QObject *parent = NULL);

	/**
	 * Funkcja akceptacji wiersza
	 *
	 * @param sourceRow wiersz
	 * @param sourceParent indeks rodzica
	 *
	 * @returns true, jeśli wiersz spełnia kryteria
	 */
	bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
};

#endif /* OBJECTINSPECTORFILTER_H_ */
