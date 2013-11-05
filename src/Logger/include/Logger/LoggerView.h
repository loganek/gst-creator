/*
 * LoggerView.h
 *
 *  Created on: 5 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef LOGGERVIEW_H_
#define LOGGERVIEW_H_

#include <QtWidgets>

class LoggerView : public QWidget
{
	Q_OBJECT

private:
	QListWidget* list;

public Q_SLOTS:
	void add_log(const QStringList& item);

public:
	explicit LoggerView(QWidget* parent = 0);
	virtual ~LoggerView();
};


#endif /* LOGGERVIEW_H_ */
