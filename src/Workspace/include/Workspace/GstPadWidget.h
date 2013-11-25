/*
 * GstPadWidget.h
 *
 *  Created on: 18 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef GSTPADWIDGET_H_
#define GSTPADWIDGET_H_

#include <QtWidgets>
#include <gstreamermm.h>

class GstPadWidget : public QLabel
{
	Q_OBJECT

private:
	Glib::RefPtr<Gst::Pad> pad;
public:
	explicit GstPadWidget(const Glib::RefPtr<Gst::Pad>& pad, QWidget* parent = 0);
	virtual ~GstPadWidget(){}

	Glib::RefPtr<Gst::Pad> get_pad();

	QPoint get_absolute_position();
};

#endif /* GSTPADWIDGET_H_ */
