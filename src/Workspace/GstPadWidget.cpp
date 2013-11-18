/*
 * GstPadWidget.cpp
 *
 *  Created on: 18 lis 2013
 *      Author: Marcin Kolny
 */

#include "GstPadWidget.h"

GstPadWidget::GstPadWidget(const Glib::RefPtr<Gst::Pad>& pad, QWidget* parent)
: pad(pad),
  QLabel(pad->get_name().c_str(), parent)
{}

Glib::RefPtr<Gst::Pad> GstPadWidget::get_pad()
{
	return pad;
}
