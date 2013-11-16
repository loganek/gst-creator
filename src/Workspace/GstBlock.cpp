/*
 * GstBlock.cpp
 *
 *  Created on: 16 lis 2013
 *      Author: Marcin Kolny
 */

#include "GstBlock.h"

using namespace Gst;
using Glib::RefPtr;

GstBlock::GstBlock(const RefPtr<Element>& element, QWidget* parent)
: QFrame(parent),
  model(element)
{
	setMinimumWidth(150);
	setMinimumHeight(50);

	QHBoxLayout* lay = new QHBoxLayout(this);
	name_label = new QLabel();
	lay->addLayout(sink_layout = new QVBoxLayout());
	lay->addWidget(name_label);
	lay->addLayout(src_layout = new QVBoxLayout());

	setStyleSheet("background-color: yellow");

	update_element_view();
}

void GstBlock::update_element_view()
{
	clear_element_view();

	name_label->setText(model->get_name().c_str());

	update_pads();
}

void clearLayout(QLayout *layout)
{
	QLayoutItem *item;
	while((item = layout->takeAt(0))) {

		if (item != nullptr && item->widget()) {
			layout->removeWidget(item->widget());
		}
	}
}

void GstBlock::clear_element_view()
{
	clearLayout(sink_layout);
	clearLayout(src_layout);
}

void GstBlock::update_pads()
{
	Iterator<Pad> iter = model->iterate_pads();

	while (iter.next())
	{
		add_pad(*iter);
	}
}

void GstBlock::add_pad(const RefPtr<Pad>& pad)
{
	QLayout* lay = (pad->get_direction() == PAD_SINK) ? sink_layout : src_layout;
	Qt::Alignment alignment = (pad->get_direction() == PAD_SINK) ? Qt::AlignLeft : Qt::AlignRight;
	alignment |= Qt::AlignVCenter;
	QLabel* lab = new QLabel(pad->get_name().c_str());
	lab->setAlignment(alignment);
	lay->addWidget(lab);
}

void GstBlock::paintEvent(QPaintEvent * event)
{
	update_element_view();
}
