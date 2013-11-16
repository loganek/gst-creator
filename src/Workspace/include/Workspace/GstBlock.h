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
	Q_OBJECT

private:
	Glib::RefPtr<Gst::Element> model;

	QLayout* sink_layout;
	QLayout* src_layout;
	QLayout* main_layout;

	QLabel* name_label;

	void update_pads();
	void add_pad(const Glib::RefPtr<Gst::Pad>& pad);
	void clear_element_view();
	void paintEvent(QPaintEvent * event);

public:
	explicit GstBlock(const Glib::RefPtr<Gst::Element>& element_model, QWidget* parent = 0);
	virtual ~GstBlock(){}

	void update_element_view();

	Glib::RefPtr<Gst::Element> get_model(){ return model; }
};

#endif /* GSTBLOCK_H_ */
