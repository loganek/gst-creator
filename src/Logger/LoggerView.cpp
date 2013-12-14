/*
 * LoggerView.cpp
 *
 *  Created on: 5 lis 2013
 *      Author: Marcin Kolny
 */

#include "LoggerView.h"
#include "utils/EnumUtils.h"

LoggerView::LoggerView(QWidget* parent)
: QWidget(parent)
{
	QHBoxLayout* lay = new QHBoxLayout();
	table = new QTableWidget();
	table->setColumnCount(2);
	table->setHorizontalHeaderItem(0, new QTableWidgetItem("Date & Time"));
	table->setColumnWidth(0, 160);
	table->horizontalHeader()->setStretchLastSection(true);
	table->setHorizontalHeaderItem(1, new QTableWidgetItem("Message"));
	lay->addWidget(table);
	setLayout(lay);
}

LoggerView::~LoggerView()
{}

void LoggerView::add_single_log(const QString& text)
{
	QStringList row = {  };
		row.append("");
		table->insertRow(table->rowCount());
		table->setItem(table->rowCount()-1, 0, new QTableWidgetItem(QDateTime::currentDateTime().toString("H:mm:ss dd/MM/yyyy")));
		table->setItem(table->rowCount()-1, 1, new QTableWidgetItem(text));
}

void LoggerView::add_log(std::shared_ptr<Command> cmd)
{
	add_single_log(QString("Executed command: ") + EnumUtils<CommandType>::enum_to_string(cmd->get_type()).c_str());
}

bool LoggerView::add_bus_log(const Glib::RefPtr<Gst::Bus>& bus, const Glib::RefPtr<Gst::Message>& message)
{
	Glib::ustring msg_type = Gst::Enums::get_name(message->get_message_type());
	add_single_log(QString("Bus message from ") +
			bus->get_name().c_str() + ": " + msg_type.c_str());

	return true;
}
