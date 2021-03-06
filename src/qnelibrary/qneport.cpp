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

#include "qneport.h"

#include <QGraphicsScene>
#include <QFontMetrics>

#include <QPen>

#include "qneconnection.h"
#include "qneblock.h"
#include "utils/GstUtils.h"

QNEPort::QNEPort(const Glib::RefPtr<Gst::Object>& model, QGraphicsItem *parent)
: QGraphicsPathItem(parent),
  model(model)
{
	label = new QGraphicsTextItem(this);

	radius_ = 5;
	margin = 2;

	QPainterPath p;
	p.addEllipse(-radius_, -radius_, 2*radius_, 2*radius_);
	setPath(p);

	setPen(QPen(Qt::darkRed));
	setBrush(Qt::gray);

	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	m_portFlags = 0;
}

QNEPort::~QNEPort()
{
	Q_FOREACH(QNEConnection *conn, m_connections)
										delete conn;
}

void QNEPort::setNEBlock(QNEBlock *b)
{
	m_block = b;
}

void QNEPort::setName(const QString &n)
{
	name = n;
	label->setPlainText(n);
}

void QNEPort::setIsOutput(bool o)
{
	isOutput_ = o;

	QFontMetrics fm(scene()->font());

	if (isOutput_)
		label->setPos(-radius_ - margin - label->boundingRect().width(), -label->boundingRect().height()/2);
	else
		label->setPos(radius_ + margin, -label->boundingRect().height()/2);
}

int QNEPort::radius()
{
	return radius_;
}

bool QNEPort::isOutput()
{
	return isOutput_;
}

QVector<QNEConnection*>& QNEPort::connections()
{
	return m_connections;
}

void QNEPort::setPortFlags(int f)
{
	m_portFlags = f;

	if (m_portFlags & TypePort)
	{
		QFont font(scene()->font());
		font.setItalic(true);
		label->setFont(font);
		setPath(QPainterPath());
	} else if (m_portFlags & NamePort)
	{
		QFont font(scene()->font());
		font.setBold(true);
		label->setFont(font);
		setPath(QPainterPath());
	}
}

QNEBlock* QNEPort::block() const
{
	return m_block;
}

quint64 QNEPort::ptr()
{
	return m_ptr;
}

void QNEPort::setPtr(quint64 p)
{
	m_ptr = p;
}

bool QNEPort::isConnected(QNEPort *other)
{
	Q_FOREACH(QNEConnection *conn, m_connections)
										if (conn->port1() == other || conn->port2() == other)
											return true;

	return false;
}

QVariant QNEPort::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemScenePositionHasChanged)
	{
		Q_FOREACH(QNEConnection *conn, m_connections)
										{
			conn->updatePosFromPorts();
			conn->updatePath();
										}
	}
	return value;
}

bool QNEPort::can_link(QNEPort* sink_port) const
{
	auto sink_model = sink_port->get_object_model();

	if (isOutput_ == sink_port->isOutput_)
		return false;
	auto m1 = block()->get_model();
	auto m2 = sink_port->block()->get_model();

	if (!model && !sink_model)
		return GstUtils::find_connection(m1, m2).exists;

	if (!model && sink_model)
		return GstUtils::find_connection(Glib::RefPtr<Gst::Object>::cast_static(m1), sink_model).exists;

	if (model && !sink_model)
		return GstUtils::find_connection(model, Glib::RefPtr<Gst::Object>::cast_static(m2)).exists;

	if (GST_IS_PAD(model->gobj()) && GST_IS_PAD(sink_model->gobj()))
		return Glib::RefPtr<Gst::Pad>::cast_static(model)->can_link(
				Glib::RefPtr<Gst::Pad>::cast_static(sink_model));

	if (GST_IS_PAD_TEMPLATE(model->gobj()) && GST_IS_PAD_TEMPLATE(sink_model->gobj()))
		return Gst::Pad::create(Glib::RefPtr<Gst::PadTemplate>::cast_static(model))->can_link(
				Gst::Pad::create(Glib::RefPtr<Gst::PadTemplate>::cast_static(sink_model)));

	if (GST_IS_PAD_TEMPLATE(model->gobj()) && GST_IS_PAD(sink_model->gobj()))
		return Gst::Pad::create(Glib::RefPtr<Gst::PadTemplate>::cast_static(model))->can_link(
				Glib::RefPtr<Gst::Pad>::cast_static(sink_model));

	if (GST_IS_PAD(model->gobj()) && GST_IS_PAD_TEMPLATE(sink_model->gobj()))
		return Glib::RefPtr<Gst::Pad>::cast_static(model)->can_link(
				Gst::Pad::create(Glib::RefPtr<Gst::PadTemplate>::cast_static(sink_model)));
	return false;
}
