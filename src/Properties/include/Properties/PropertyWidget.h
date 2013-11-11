/*
 * PropertyWidget.h
 *
 *  Created on: 11 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef PROPERTYWIDGET_H_
#define PROPERTYWIDGET_H_

#include <QtWidgets>
#include "Property.h"
#include <memory>

class PropertyWidget : public QWidget
{
	Q_OBJECT
private:
	std::vector<std::shared_ptr<Property>> properties;

	void save_properties();
public:
	PropertyWidget(QWidget* parent = 0);
	void add_property(Property* property);
};

#endif /* PROPERTYWIDGET_H_ */
