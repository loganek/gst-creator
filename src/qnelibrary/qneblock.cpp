/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
 * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include "qneblock.h"

#include <QPen>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>

#include "qneport.h"

QNEBlock::QNEBlock(const Glib::RefPtr<Gst::Element>& model, QGraphicsItem *parent)
: QGraphicsPathItem(parent),
  model(model)
{
	QPainterPath p;
	p.addRoundedRect(-50, -15, 100, 30, 5, 5);
	setPath(p);
	setPen(QPen(Qt::darkGreen));
	setBrush(Qt::green);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	horzMargin = 20;
	vertMargin = 5;
	width = horzMargin;
	height = vertMargin;
}

QNEPort* QNEBlock::addPort(const Glib::RefPtr<Gst::Object>& model, bool isOutput, int flags, int ptr)
{
	QNEPort *port = new QNEPort(model, this);
	Glib::ustring port_name = model ? model->get_name() : (
			isOutput ? "ai_src" : "ai_sink");
	port->setName(port_name.c_str());
	port->setIsOutput(isOutput);
	port->setNEBlock(this);
	port->setPortFlags(flags);
	port->setPtr(ptr);

	QFontMetrics fm(scene()->font());
	int w = fm.width(port_name.c_str());
	int h = fm.height();
	// port->setPos(0, height + h/2);
	if (w > width - horzMargin)
		width = w + horzMargin;
	height += h;

	QPainterPath p;
	p.addRoundedRect(-width/2, -height/2, width, height, 5, 5);
	setPath(p);

	int y = -height / 2 + vertMargin + port->radius();
	Q_FOREACH(QGraphicsItem *port_, childItems()) {
		if (port_->type() != QNEPort::Type)
			continue;

		QNEPort *port = (QNEPort*) port_;
		if (port->isOutput())
			port->setPos(width/2 + port->radius(), y);
		else
			port->setPos(-width/2 - port->radius(), y);
		y += h;
	}

	return port;
}

void QNEBlock::addInputPort(const Glib::RefPtr<Gst::Object>& model)
{
	addPort(model, false);
}

void QNEBlock::addOutputPort(const Glib::RefPtr<Gst::Object>& model)
{
	addPort(model, true);
}

void QNEBlock::save(QDataStream &ds)
{
	ds << pos();

	int count(0);

	Q_FOREACH(QGraphicsItem *port_, childItems())
	{
		if (port_->type() != QNEPort::Type)
			continue;

		count++;
	}

	ds << count;

	Q_FOREACH(QGraphicsItem *port_, childItems())
	{
		if (port_->type() != QNEPort::Type)
			continue;

		QNEPort *port = (QNEPort*) port_;
		ds << (quint64) port;
		ds << port->portName();
		ds << port->isOutput();
		ds << port->portFlags();
	}
}

void QNEBlock::load(QDataStream &ds, QMap<quint64, QNEPort*> &portMap)
{
	QPointF p;
	ds >> p;
	setPos(p);
	int count;
	ds >> count;
	for (int i = 0; i < count; i++)
	{
		QString name;
		bool output;
		int flags;
		quint64 ptr;

		ds >> ptr;
		ds >> name;
		ds >> output;
		ds >> flags;
		// portMap[ptr] = addPort(name, output, flags, ptr); fixme is this function needed?
	}
}

#include <QStyleOptionGraphicsItem>

void QNEBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
							Q_UNUSED(widget)

							if (isSelected()) {
								painter->setPen(QPen(Qt::darkYellow));
								painter->setBrush(Qt::yellow);
							} else {
								painter->setPen(QPen(Qt::darkGreen));
								painter->setBrush(Qt::green);
							}

	painter->drawPath(path());
}

QNEBlock* QNEBlock::clone()
{
	QNEBlock *b = new QNEBlock(model, 0);
	this->scene()->addItem(b);

	Q_FOREACH(QGraphicsItem *port_, childItems())
	{
		if (port_->type() == QNEPort::Type)
		{
			QNEPort *port = (QNEPort*) port_;
			b->addPort(port->get_object_model(), port->isOutput(), port->portFlags(), port->ptr());
		}
	}

	return b;
}

QVector<QNEPort*> QNEBlock::ports()
{
	QVector<QNEPort*> res;
	Q_FOREACH(QGraphicsItem *port_, childItems())
	{
		if (port_->type() == QNEPort::Type)
			res.append((QNEPort*) port_);
	}
	return res;
}

QVariant QNEBlock::itemChange(GraphicsItemChange change, const QVariant &value)
{

	Q_UNUSED(change);

	return value;
}

QNEPort* QNEBlock::find_port(const Glib::RefPtr<Gst::Object>& model)
{
	auto items = childItems();
	for (auto item : items)
		if (item && item->type() == QNEPort::Type)
		{
			QNEPort* port = static_cast<QNEPort*>(item);
			if (port->get_object_model() == model)
				return port;
		}

	return nullptr;
}
