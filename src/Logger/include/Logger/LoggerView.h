/*
 * LoggerView.h
 *
 *  Created on: 5 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef LOGGERVIEW_H_
#define LOGGERVIEW_H_

#include <QtWidgets>
#include <memory>
#include <gstreamermm.h>
#include "Commands/Command.h"

class LoggerView : public QWidget
{
	Q_OBJECT

private:
	QTableWidget* table;

	void add_single_log(const QString& text);

public Q_SLOTS:
	void add_log(std::shared_ptr<Command> cmd);
	bool add_bus_log(const Glib::RefPtr<Gst::Bus>& bus, const Glib::RefPtr<Gst::Message>& message);

public:
	explicit LoggerView(QWidget* parent = 0);
	virtual ~LoggerView();
};


#endif /* LOGGERVIEW_H_ */
