/*
 * GstBlock.cpp
 *
 *  Created on: 14 pa? 2013
 *      Author: Marcin Kolny
 */

#include "GstBlock.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

using namespace Gst;
using Glib::RefPtr;

GstBlock::GstBlock(const char* element_name, QWidget *parent)
: QFrame(parent)
{
	setup_layout();

	setMinimumWidth(200);
	setMinimumHeight(100);

	RefPtr<Element> element = ElementFactory::create_element(element_name);

	Iterator<Pad> p = element->iterate_pads();

	while(p.next())
	{
		if (p->get_direction() == Gst::PAD_SRC)
		{
			add_pad(p->get_name().c_str(), PadDir::OUTPUT);
		}
		else if (p->get_direction() == Gst::PAD_SINK)
		{
			add_pad(p->get_name().c_str(), PadDir::INPUT);
		}
	}
}

GstBlock::~GstBlock()
{
	delete name_label;
	delete right_layout;
	delete left_layout;
}

void GstBlock::setup_layout()
{
	QHBoxLayout* main_layout = new QHBoxLayout(this);

	main_layout->addLayout(left_layout = new QVBoxLayout);

	main_layout->addWidget(name_label = new QLabel("UNKNOW"));
	name_label->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

	main_layout->addLayout(right_layout = new QVBoxLayout);
}

void GstBlock::set_name(const QString& name)
{
	name_label->setText(name);
}

void GstBlock::add_pad(const QString& pad_name, PadDir dir)
{
	QLayout* layout;
	Qt::Alignment alignment;

	if (dir == PadDir::INPUT)
	{
		layout = left_layout;
		alignment = Qt::AlignLeft;
	}
	else
	{
		layout = right_layout;
		alignment = Qt::AlignRight;
	}

	alignment |= Qt::AlignVCenter;

	QLabel* label = new QLabel(pad_name);
	label->setAlignment(alignment);
	layout->addWidget(label);
}

void GstBlock::remove_pad(const QString& pad_name)
{
	for (int i = 0; i < left_layout->count(); i++)
	{
		QLayoutItem* item = left_layout->itemAt(i);
		QLabel* label = dynamic_cast<QLabel*>(item->widget());

		if (label && label->text() == pad_name)
		{
			delete item->widget();
			return;
		}
	}
}


