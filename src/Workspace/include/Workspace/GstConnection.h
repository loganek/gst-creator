/*
 * GstConnection.h
 *
 *  Created on: 18 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef GSTCONNECTION_H_
#define GSTCONNECTION_H_

#include "GstPadWidget.h"

enum class ConnectState
{
	OK_CONNECTION,
	BAD_CONNECTION,
	NO_CONNECTION
};

class GstConnection
{
private:
	GstPadWidget* first_pad;
	GstPadWidget* second_pad;

public:
	GstConnection(GstPadWidget* first_pad,GstPadWidget* second_pad);

	GstPadWidget* get_first_pad();
	GstPadWidget* get_second_pad();

	static void draw_arrow(QPainter& painter, const QPoint& start, const QPoint& end, ConnectState state = ConnectState::NO_CONNECTION);
	void draw_arrow(QPainter& painter);
	bool may_exists();
};

#endif /* GSTCONNECTION_H_ */
