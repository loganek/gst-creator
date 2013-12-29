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
	QVBoxLayout* lay = new QVBoxLayout();
	QGroupBox* frame = new QGroupBox("Log filter");
	frame->setStyleSheet("QGroupBox{border: 1px solid gray; border-radius: 1px;padding-top:10px}"
			"QGroupBox::title {background-color: transparent; subcontrol-position: top left;padding:2 13px;}");
	frame->setLayout(new QHBoxLayout());
	frame->layout()->addWidget(msg_cbox = new QCheckBox("Bus messages"));
	msg_cbox->setChecked(true);
	frame->layout()->addWidget(cmd_cbox = new QCheckBox("Commands"));
	cmd_cbox->setChecked(true);
	lay->addWidget(frame);
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
	table->insertRow(0);
	table->setItem(0, 0, new QTableWidgetItem(QDateTime::currentDateTime().toString("H:mm:ss dd/MM/yyyy")));
	table->setItem(0, 1, new QTableWidgetItem(text));
}

void LoggerView::add_log(std::shared_ptr<Command> cmd)
{
	if (cmd_cbox->isChecked())
		add_single_log(QString("Executed command: ") + EnumUtils<CommandType>::enum_to_string(cmd->get_type()).c_str());
}

bool LoggerView::add_bus_log(const Glib::RefPtr<Gst::Bus>& bus, const Glib::RefPtr<Gst::Message>& message)
{
	Glib::ustring msg_type = Gst::Enums::get_name(message->get_message_type());
	if (msg_cbox->isChecked())
	{
		std::string msg_text;

		switch (message->get_message_type())
		{
		case Gst::MESSAGE_ERROR:
		{
			Glib::Error err;
			Glib::RefPtr<Gst::MessageError>::cast_static(message)->parse(err, msg_text);
			msg_text += std::string(", Error: ") + err.what().c_str();
			break;
		}
		case Gst::MESSAGE_STATE_CHANGED:
		{
			Gst::State old_state, new_state, pending;
			Glib::RefPtr<Gst::MessageStateChanged>::cast_static(message)->parse(old_state, new_state, pending);
			msg_text = (Gst::Enums::get_name(old_state) + " -> " + Gst::Enums::get_name(new_state)).c_str();
			break;
		}
		default:
			msg_text = "unknown message";
		}

		add_single_log(QString("Bus message from ") +
				bus->get_name().c_str() + ": " + msg_type.c_str() + ": " + msg_text.c_str());
	}
	return true;
}
