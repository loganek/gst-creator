/*
 * LoggerView.cpp
 *
 *  Created on: 5 lis 2013
 *      Author: Marcin Kolny
 */

#include "LoggerView.h"

LoggerView::LoggerView(QWidget* parent)
: QWidget(parent)
{
	QHBoxLayout* lay = new QHBoxLayout();
	list = new QListWidget();
	lay->addWidget(list);
	setLayout(lay);
}

LoggerView::~LoggerView()
{}

void LoggerView::add_log(const QStringList& item)
{
	list->addItems(item);
}
