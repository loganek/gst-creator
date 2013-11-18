/*
 * GstPadWidget.cpp
 *
 *  Created on: 18 lis 2013
 *      Author: Marcin Kolny
 */

#include "GstPadWidget.h"
#include "GstBlock.h"

GstPadWidget::GstPadWidget(const Glib::RefPtr<Gst::Pad>& pad, QWidget* parent)
: pad(pad),
  QLabel(pad->get_name().c_str(), parent)
{}

Glib::RefPtr<Gst::Pad> GstPadWidget::get_pad()
{
	return pad;
}

QPoint GstPadWidget::get_absolute_position()
{
	return static_cast<GstBlock*>(parent())->get_pad_point(this);
}
