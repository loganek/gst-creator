/*
 * ObjectInspectorModel.h
 *
 * Plik zawiera definicję klasy ObjectInspectorModel
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

/**
 * Klasa reprezentuje model inspektora obiektów
 */
class ObjectInspectorModel : public QAbstractItemModel
{
private:
	/**
	 * Konfiguracja danych modelu
	 */
	void setup_model_data();

	/** Tablica obiektów w modelu */
	std::vector<ObjectInspectorItem*> items;
	/** Obiekt korzenia */
	ObjectInspectorItem* root_item;

	/**
	 * Dodaje wtyczkę do modelu
	 *
	 * @param plugin model wtyczki
	 */
	void add_plugin_to_model(const Glib::RefPtr<Gst::Plugin>& plugin);

	Q_OBJECT

public:
	/**
	 * Konstruktor
	 *
	 * @param parent rodzic okna dialogowego
	 */
	explicit ObjectInspectorModel(QObject *parent = 0);

	/**
	 * Destruktor
	 */
	~ObjectInspectorModel();

	/**** Implementacja klasy QAbstractItemModel ****/
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
