/*
 * ConsoleView.h
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef CONSOLEVIEW_H_
#define CONSOLEVIEW_H_

#include "Console/CommandParser.h"
#include <QWidget>
#include <QtWidgets>
#include <gstreamermm.h>

class ConsoleView : public QWidget
{
	Q_OBJECT
private:
	QLineEdit* edit;
	QPushButton* button;

	Glib::RefPtr<Gst::Pipeline> model;

	CommandParser* parser;
private Q_SLOTS:
	void execute_command();

Q_SIGNALS:
	void commandAdded(const QStringList& item);

public:
	explicit ConsoleView(QWidget* parent = 0);
	virtual ~ConsoleView();
	void set_model(const Glib::RefPtr<Gst::Pipeline>& model);
};

#endif /* CONSOLEVIEW_H_ */
