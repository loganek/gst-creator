/*
 * PropertyWidget.h
 *
 * Plik zawiera deklarację klasy PropertyWidget
 *
 *  Created on: 11 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef PROPERTYWIDGET_H_
#define PROPERTYWIDGET_H_

#include <QtWidgets>
#include "Property.h"
#include <memory>

/**
 * Klasa widgeta umożliwiającego edycję kilku parametrów na raz.
 */
class PropertyWidget : public QWidget
{
	Q_OBJECT
private:
	/** lista właściwości */
	std::vector<std::shared_ptr<Property>> properties;

	/**
	 * Zapisuje wszystkie właściwości
	 */
	void save_properties();
public:
	/**
	 * Konstruktor, inicjalizuje pola klasy
	 *
	 * @param parent rodzic widgeta
	 */
	PropertyWidget(QWidget* parent = 0);

	/**
	 * Dodaje właściwość do listy właściwości
	 *
	 * @param property właściwość, jaką należy dodać
	 */
	void add_property(Property* property);
};

#endif /* PROPERTYWIDGET_H_ */
