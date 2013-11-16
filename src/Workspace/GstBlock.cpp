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
	setLayout(new QHBoxLayout());
	sink_frame = new QFrame(this);
	sink_frame->setLayout(new QVBoxLayout());
	sink_frame->layout()->addWidget(new QLabel("no shit, cpt obvious"));
	src_frame = new QFrame(this);
	main_frame = new QFrame(this);
}
