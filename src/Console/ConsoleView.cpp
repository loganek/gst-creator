/*
 * ConsoleView.cpp
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#include "ConsoleView.h"
#include <QDebug>

ConsoleView::ConsoleView(QWidget* parent)
: QWidget(parent)
{
	edit = new QLineEdit();
	button = new QPushButton("Execute");

	QHBoxLayout* lay = new QHBoxLayout();

	lay->addWidget(edit);
	lay->addWidget(button);

	QObject::connect(button, &QPushButton::clicked, this, &ConsoleView::execute_command);

	setLayout(lay);
}

void ConsoleView::execute_command()
{

}
