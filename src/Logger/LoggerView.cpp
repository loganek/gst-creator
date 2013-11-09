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
	table = new QTableWidget();
	table->setColumnCount(2);
	table->setHorizontalHeaderItem(0, new QTableWidgetItem("Date & Time"));
	table->setHorizontalHeaderItem(1, new QTableWidgetItem("Message"));
	lay->addWidget(table);
	setLayout(lay);
}

LoggerView::~LoggerView()
{}

void LoggerView::add_log(const QStringList& item)
{
	QStringList row = {  };
	row.append(item);
	table->insertRow(table->rowCount());
	table->setItem(table->rowCount()-1, 0, new QTableWidgetItem(QDate::currentDate().toString("d-m-Y h:m:s")));
	table->setItem(table->rowCount()-1, 1, new QTableWidgetItem(item[0]));
}
