/*
 * ObjectInspectorView.h
 *
 * Plik zawiera deklarację klasy ObjectInspectorView
 *
 *  Created on: 28 paź 2013
 *      Author: Marcin Kolny
 */

#ifndef OBJECTINSPECTORVIEW_H_
#define OBJECTINSPECTORVIEW_H_

#include <QTreeView>
#include <QPoint>

/**
 * Klasa reprezentująca drzewo wyświetlające dostępne wtyczki
 */
class ObjectInspectorView : public QTreeView
{
	Q_OBJECT

	private Q_SLOTS:
	/**
	 * Slot kliknięcia w drzewo
	 *
	 * @param index indeks obiektu, na który kliknięto
	 */
	void on_object_inspector_clicked(const QModelIndex &index);

	/**
	 * Slot podwójnego kliknięcia w drzewo
	 *
	 * @param index indeks obiektu, na który kliknięto
	 */
	void on_object_inspector_double_clicked(const QModelIndex &index);

	private:
	/** nazwa aktualnie zaznaczonego obiektu */
	QString current_text;
	/** pozycja aktualnie zaznaczonego obiektu */
	QPoint current_location;

	protected:
	/**
	 * Rozpoczyna proces przeciągania elementu
	 */
	void startDrag(Qt::DropActions supportedActions);

	public:
	ObjectInspectorView(QWidget *parent = 0);
	virtual ~ObjectInspectorView();
};

#endif /* OBJECTINSPECTORVIEW_H_ */
