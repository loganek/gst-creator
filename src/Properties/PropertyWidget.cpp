/*
 * PropertyWidget.cpp
 *
 *  Created on: 11 lis 2013
 *      Author: Marcin Kolny
 */

#include "PropertyWidget.h"

PropertyWidget::PropertyWidget(QWidget* parent)
: QWidget(parent)
{
	QPushButton* ok_button = new QPushButton("OK");
	QPushButton* cancel_button = new QPushButton("Cancel");

	setLayout(new QVBoxLayout());
	layout()->addWidget(ok_button);
	layout()->addWidget(cancel_button);

	QObject::connect(cancel_button, &QPushButton::clicked, [&](bool) {
		if (parentWidget())
			parentWidget()->close();
		else
			close();
	} );
	QObject::connect(ok_button, &QPushButton::clicked, [&](bool) {
		save_properties();
		if (parentWidget())
			parentWidget()->close();
		else
			close();
	} );
}

void PropertyWidget::add_property(Property* property)
{
	layout()->addWidget(property->get_widget());
	properties.push_back(std::shared_ptr<Property>(property));
}

void PropertyWidget::save_properties()
{
	for (auto& property : properties)
	{
		property->set_value();
	}
}


