/*
 * GstConnection.cpp
 *
 *  Created on: 18 lis 2013
 *      Author: Marcin Kolny
 */

#include "GstConnection.h"
#include <cmath>

using namespace Gst;
using Glib::RefPtr;

GstConnection::GstConnection(GstPadWidget* first_pad, GstPadWidget* second_pad)
: first_pad(first_pad),
  second_pad(second_pad)
{
}

GstPadWidget* GstConnection::get_first_pad()
{
	return first_pad;
}

GstPadWidget* GstConnection::get_second_pad()
{
	return second_pad;
}

void GstConnection::draw_arrow(QPainter& painter)
{
	QPoint end = second_pad->get_absolute_position(),
			start = first_pad->get_absolute_position();

	draw_arrow(painter, start, end);
}

void GstConnection::draw_arrow(QPainter& painter, const QPoint& start, const QPoint& end, ConnectState state)
{
	double len = 10, rads = 35 * M_PI/180;
	double angle = atan2 (end.y() - start.y(), end.x() - start.x()) + M_PI;

	QPoint arr1(end.x() + len * cos(angle - rads),
			end.y() + len * sin(angle - rads));
	QPoint arr2(end.x() + len * cos(angle + rads),
			end.y() + len * sin(angle + rads));

	Qt::GlobalColor color;
	switch (state)
	{
	case ConnectState::OK_CONNECTION:
		color = Qt::green;
		break;
	case ConnectState::BAD_CONNECTION:
		color = Qt::red;
		break;
	default:
		color = Qt::black;
		break;
	}

	painter.setPen(QPen(color, 2));

	painter.drawLine(QLine(start, end));
	painter.drawLine(QLine(arr1, end));
	painter.drawLine(QLine(arr2, end));
}

bool GstConnection::may_exists()
{
	return first_pad->get_pad()->can_link(second_pad->get_pad());
}
