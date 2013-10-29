/*
 * ConsoleView.h
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef CONSOLEVIEW_H_
#define CONSOLEVIEW_H_

#include <QWidget>
#include <QtWidgets>

class ConsoleView : public QWidget
{
	Q_OBJECT
private:
	QLineEdit* edit;
	QPushButton* button;

private Q_SLOTS:
	void execute_command();

public:
	explicit ConsoleView(QWidget* parent = 0);
};

#endif /* CONSOLEVIEW_H_ */
