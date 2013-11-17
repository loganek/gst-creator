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
#include "Commands/Command.h"

class LoggerView : public QWidget
{
	Q_OBJECT

private:
	QTableWidget* table;

public Q_SLOTS:
	void add_log(std::shared_ptr<Command> cmd);

public:
	explicit LoggerView(QWidget* parent = 0);
	virtual ~LoggerView();
};


#endif /* LOGGERVIEW_H_ */
