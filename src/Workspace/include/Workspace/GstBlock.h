/*
 * GstBlock.h
 *
 *  Created on: 16 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef GSTBLOCK_H_
#define GSTBLOCK_H_

#include <QtWidgets>
#include <gstreamermm.h>

class GstBlock : public QFrame
{
	Glib::RefPtr<Gst::Element> model;

	QFrame* sink_frame;
	QFrame* src_frame;
	QFrame* main_frame;

public:
	explicit GstBlock(const Glib::RefPtr<Gst::Element>& element_model, QWidget* parent = 0);
	virtual ~GstBlock(){}
};

#endif /* GSTBLOCK_H_ */
