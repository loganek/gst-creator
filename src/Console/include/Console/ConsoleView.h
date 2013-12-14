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
#include <memory>

class ConsoleView : public QWidget
{
	Q_OBJECT
private:
	QLineEdit* edit;
	QPushButton* button;
	QTreeWidget* suggestions;
	std::vector<CommandListener*> listeners;
	Glib::RefPtr<Gst::Pipeline> model;

	CommandParser* parser;
	std::vector<std::string> command_args;

	std::vector<std::string> get_current_suggestions(std::vector<std::string> command_args);
	void make_suggestion_widget();
	bool eventFilter(QObject *obj, QEvent *ev);
	void make_suggestion(const QString& text, bool autocomplete = false);
private Q_SLOTS:
	void execute_command();
	void suggest(const QString& text);


Q_SIGNALS:
	void command_added(std::shared_ptr<Command> command);

public:
	explicit ConsoleView(std::vector<CommandListener*> listeners = {}, QWidget* parent = 0);
	virtual ~ConsoleView();
	void set_model(const Glib::RefPtr<Gst::Pipeline>& model);
};

#endif /* CONSOLEVIEW_H_ */
