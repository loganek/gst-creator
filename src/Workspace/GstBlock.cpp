/*
 * GstBlock.cpp
 *
 *  Created on: 16 lis 2013
 *      Author: Marcin Kolny
 */

#include "GstBlock.h"
#include "GstPadWidget.h"
#include <algorithm>

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
	pads.clear();
}

void GstBlock::update_pads()
{
	Iterator<Pad> iter = model->iterate_pads();

	while (iter.next())
	{
		add_pad(*iter);
	}
	setFixedHeight(std::max(get_sink_cnt(), get_src_cnt()) * 30);
}

void GstBlock::add_pad(const RefPtr<Pad>& pad)
{
	QLayout* lay = (pad->get_direction() == PAD_SINK) ? sink_layout : src_layout;
	Qt::Alignment alignment = (pad->get_direction() == PAD_SINK) ? Qt::AlignLeft : Qt::AlignRight;
	alignment |= Qt::AlignVCenter;
	GstPadWidget* lab = new GstPadWidget(pad);
	lab->setAlignment(alignment);
	pads.push_back(lab);
	lay->addWidget(lab);
}

void GstBlock::paintEvent(QPaintEvent * event)
{
	update_element_view();
}

GstPadWidget* GstBlock::find_pad(QPoint pt)
{
	int sinks = 0, srcs = 0, adder;

	for (auto pad : pads)
	{
		adder = 30 - pad->grab().height(); // TODO magic numbers!:)
		if (pad->get_pad()->get_direction() == PAD_SINK)
		{
			int cur_y = sinks * grab().height() / get_sink_cnt();
			if (pad->grab().width() > pt.x() && (pt.y() > cur_y && cur_y + pad->grab().height() + adder > pt.y()))
				return pad;

			sinks++;
		}
		else if (pad->get_pad()->get_direction() == PAD_SRC)
		{
			int cur_y = srcs * grab().height() / get_src_cnt();
			if (grab().width() - pad->grab().width() < pt.x() && (pt.y() > cur_y && cur_y + pad->grab().height() + adder > pt.y()))
				return pad;

			srcs++;
		}
	}

	return nullptr;
}

int GstBlock::get_sink_cnt()
{
	int cnt = 0;

	for (auto pad : pads)
		if (pad->get_pad()->get_direction() == PAD_SINK)
			cnt++;

	return cnt;
}

int GstBlock::get_src_cnt()
{
	return pads.size() - get_sink_cnt();
}
