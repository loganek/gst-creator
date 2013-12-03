/*
 * WorkspaceWidget.cpp
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#include "WorkspaceWidget.h"
#include "common.h"
#include "utils/GstUtils.h"
#include <QtGui>
#include <QFrame>

using namespace Gst;
using Glib::RefPtr;

WorkspaceWidget::WorkspaceWidget(const RefPtr<Pipeline>& model, QWidget* parent)
: QWidget(parent),
  model(model),
  current_connection(nullptr)
{
	setAcceptDrops(true);
	scene = new QGraphicsScene();
	scene->installEventFilter(this);
	view = new QGraphicsView(scene, this);
	view->setRenderHint(QPainter::Antialiasing, true);
	this->installEventFilter(this);
	CommandListener::refcount++;
}

WorkspaceWidget::~WorkspaceWidget()
{
	CommandListener::refcount--;
}

void WorkspaceWidget::resizeEvent(QResizeEvent * event)
{
	view->resize(event->size());
}

bool WorkspaceWidget::check_mime_data(const QMimeData* mime_data) const
{
	return mime_data->hasFormat(DRAG_DROP_FORMAT);
}

QString WorkspaceWidget::get_new_name(const QString& name)
{
	bool ok;
	QString new_name = QInputDialog::getText(this, tr("QInputDialog::getText()"),
			tr("Element name:"), QLineEdit::Normal, name, &ok);
	if (ok && !new_name.isEmpty())
		return new_name;
	else
		return QString();
}

QGraphicsItem* WorkspaceWidget::item_at(const QPointF &pos)
{
	QList<QGraphicsItem*> items = scene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

	Q_FOREACH(QGraphicsItem *item, items)
	if (item->type() > QGraphicsItem::UserType)
		return item;

	return nullptr;
}
QPointF last_point; // fixme
bool WorkspaceWidget::eventFilter(QObject *o, QEvent *e)
{
	QGraphicsSceneMouseEvent *me = (QGraphicsSceneMouseEvent*) e;

	switch ((int) e->type())
	{
	case QEvent::GraphicsSceneMousePress:
	{
		switch ((int) me->button())
		{
		case Qt::LeftButton:
		{
			QGraphicsItem *item = item_at(me->scenePos());
			if (!item || item->type() != QNEPort::Type)
				break;

			current_connection = new QNEConnection(0);
			scene->addItem(current_connection);
			current_connection->setPort1((QNEPort*) item);
			current_connection->setPos1(item->scenePos());
			current_connection->setPos2(me->scenePos());
			current_connection->updatePath();

			return true;
		}
		case Qt::RightButton:
		{
			QGraphicsItem *item = item_at(me->scenePos());

			if (item && (item->type() == QNEConnection::Type))
			{
				QNEConnection* con = static_cast<QNEConnection*>(item);
				QNEPort* src_port = con->port1()->isOutput() ? con->port1() : con->port2();
				QNEPort* sink_port = con->port1()->isOutput() ? con->port2() : con->port1();

				DisconnectCommand cmd(src_port->get_object_model(), sink_port->get_object_model());
				cmd.run_command(this);
			}

			else if (item && (item->type() == QNEBlock::Type))
			{
				QNEBlock* block = static_cast<QNEBlock*>(item);
				RemoveCommand cmd(ObjectType::ELEMENT, block->get_model());
				cmd.run_command(this);
			}
			break;
		}
		}
		break;
	}
	case QEvent::GraphicsSceneMouseMove:
	{
		if (!current_connection)
			break;

		current_connection->setPos2(me->scenePos());
		current_connection->updatePath();
		QGraphicsItem *item = item_at(me->scenePos());
		if (item && item != current_connection->port1() && item->type() == QNEPort::Type)
		{
			QNEPort *src_port = (current_connection->port1()->isOutput()) ? current_connection->port1() : (QNEPort*) item;
			QNEPort *sink_port = (!current_connection->port1()->isOutput()) ? current_connection->port1() : (QNEPort*) item;

			current_connection->connectColor(src_port->can_link(sink_port));
		}
		else
			current_connection->connectColor(2);

		return true;
	}
	case QEvent::GraphicsSceneDragMove:
	{
		QGraphicsSceneDragDropEvent* ev = static_cast<QGraphicsSceneDragDropEvent*>(e);
		if (ev == nullptr || !check_mime_data(ev->mimeData()))
		{
			e->ignore();
			return false;
		}
		e->accept();
		return true;
	}
	case QEvent::GraphicsSceneMouseRelease:
	{
		if (!current_connection || me->button() != Qt::LeftButton)
			break;

		QGraphicsItem *item = item_at(me->scenePos());
		if (item && item->type() == QNEPort::Type)
		{
			QNEPort *src_port = (current_connection->port1()->isOutput()) ? current_connection->port1() : (QNEPort*) item;
			QNEPort *sink_port = (!current_connection->port1()->isOutput()) ? current_connection->port1() : (QNEPort*) item;

			delete current_connection;
			current_connection = 0;

			if (!src_port->can_link(sink_port))
				return true;

			if (src_port->block() != sink_port->block() && src_port->isOutput() != sink_port->isOutput() && !src_port->isConnected(sink_port))
			{
				if (!src_port->get_object_model() && !sink_port->get_object_model())
				{
					ConnectCommand cmd (src_port->block()->get_model(), sink_port->block()->get_model());
					cmd.run_command(this);
					return true;
				}

				RefPtr<Pad> src_pad = src_pad.cast_static(src_port->get_object_model());
				RefPtr<Pad> sink_pad = sink_pad.cast_static(sink_port->get_object_model());
				if (src_port->is_template_model() && RefPtr<PadTemplate>::cast_static(src_port->get_object_model())->get_presence() == PAD_REQUEST)
				{
					auto tpl = RefPtr<PadTemplate>::cast_static(src_port->get_object_model());
					AddCommand add_cmd(ObjectType::PAD, src_port->block()->get_model(), tpl);
					src_pad = src_pad.cast_static(add_cmd.run_command_ret(this));
				}
				else if (sink_port->is_template_model() && RefPtr<PadTemplate>::cast_static(sink_port->get_object_model())->get_presence() == PAD_REQUEST)
				{
					auto tpl = RefPtr<PadTemplate>::cast_static(sink_port->get_object_model());
					AddCommand add_cmd(ObjectType::PAD, sink_port->block()->get_model(), tpl);
					sink_pad = sink_pad.cast_static(add_cmd.run_command_ret(this));
				}
				if (src_port->is_template_model() && RefPtr<PadTemplate>::cast_static(src_port->get_object_model())->get_presence() == PAD_SOMETIMES)
				{
					auto pad_parent = src_port->block()->get_model();
					auto tpl = RefPtr<PadTemplate>::cast_static(src_port->get_object_model());
					ConnectCommand con_cmd(tpl, pad_parent, sink_pad);
					con_cmd.run_command(this);
					return true;
				}
				ConnectCommand cmd(src_pad, sink_pad);
				cmd.run_command(this);


				return true;
			}
		}

		delete current_connection;
		current_connection = 0;
		return true;
	}
	case QEvent::GraphicsSceneDrop:
	{
		QGraphicsSceneDragDropEvent* ev = static_cast<QGraphicsSceneDragDropEvent*>(e);
		if (ev == nullptr || !check_mime_data(ev->mimeData()))
		{
			e->ignore();
			return false;
		}

		QByteArray item_data = ev->mimeData()->data(DRAG_DROP_FORMAT);
		QDataStream data_stream(&item_data, QIODevice::ReadOnly);
		QString name;
		data_stream >> name;

		RefPtr<Element> element = ElementFactory::create_element(name.toUtf8().constData());
		name = get_new_name(element->get_name().c_str());

		if (name.isEmpty())
			return true;

		element->set_name(name.toUtf8().constData());
		last_point = me->scenePos();
		AddCommand cmd(ObjectType::ELEMENT, model, element);
		cmd.run_command(this);
		return true;
	}
	}
	return QObject::eventFilter(o, e);
}

void WorkspaceWidget::new_element_added(const RefPtr<Element>& element)
{
	QNEBlock *b = new QNEBlock(element, 0);
	scene->addItem(b);
	b->addPort(element, 0, QNEPort::NamePort);
	if (!GstUtils::is_src_element(element))
		b->addInputPort(RefPtr<Object>());
	if (!GstUtils::is_sink_element(element))
		b->addOutputPort(RefPtr<Object>());

	auto pad_iterator = element->iterate_pads();

	while (pad_iterator.next())
	{
		if (pad_iterator->get_direction() == PAD_SINK)
			b->addInputPort(*pad_iterator);
		else if (pad_iterator->get_direction() == PAD_SRC)
			b->addOutputPort(*pad_iterator);
	}

	std::vector<StaticPadTemplate> templates = element->get_factory()->get_static_pad_templates();

	for (auto tpl : templates)
	{
		if (tpl.get_presence() == PAD_SOMETIMES || tpl.get_presence() == PAD_REQUEST)
			b->addPort(element->get_pad_template(tpl.get_name_template()), tpl.get_direction() == PAD_SRC);
	}

	b->setPos(last_point);
}

void WorkspaceWidget::element_removed(const RefPtr<Element>& element)
{
	QNEBlock *b = find_block(element);
	delete b;
}

QNEPort* WorkspaceWidget::find_port(const RefPtr<Pad>& pad)
{
	QNEBlock* block = find_block(pad->get_parent_element());
	return (!block) ? nullptr : block->find_port(pad);
}

QNEPort* WorkspaceWidget::find_port(const RefPtr<PadTemplate>& pad_template, const RefPtr<Element>& parent)
{
	QNEBlock* block = find_block(parent);
	return (!block) ? nullptr : block->find_port(pad_template);
}

QNEBlock* WorkspaceWidget::find_block(const RefPtr<Element>& element)
{
	for (auto item : scene->items())
	{
		if (!item || item->type() != QNEBlock::Type)
			continue;

		QNEBlock* block = static_cast<QNEBlock*>(item);

		if (block && block->get_model() == element)
			return block;
	}

	return nullptr;
}

void WorkspaceWidget::pad_added(const RefPtr<Pad>& pad)
{
	QNEBlock* block = find_block(pad->get_parent_element());

	if (block == nullptr)
		return;

	if (pad->get_direction() == PAD_SINK)
		block->addInputPort(pad);
	else if (pad->get_direction() == PAD_SRC)
		block->addOutputPort(pad);
}

void WorkspaceWidget::pad_linked(const RefPtr<Pad>& pad)
{
	if (pad->get_direction() == PAD_SINK)
		return;

	QNEPort* first_port = find_port(pad->get_peer()),
			*second_port = find_port(pad);

	if (!first_port || !second_port)
		return;

	QNEConnection* connection = new QNEConnection(0);
	scene->addItem(connection);
	connection->setPos2(second_port->scenePos());
	connection->setPort2(second_port);
	connection->setPos1(first_port->scenePos());
	connection->setPort1(first_port);
	connection->updatePath();
}

void WorkspaceWidget::pad_removed(const RefPtr<Pad>& pad)
{
	QNEPort* port = find_port(pad);
	delete port;
}

void WorkspaceWidget::pad_unlinked(const RefPtr<Pad>& pad)
{
	if (pad->get_direction() == PAD_SINK)
		return;

	QNEPort* port = find_port(pad);

	if (port == nullptr)
		return;

	for (auto item : scene->items())
	{
		if (!item || item->type() != QNEConnection::Type)
			continue;

		QNEConnection* con = static_cast<QNEConnection*>(item);

		if (con->port1() && con->port1()->get_object_model() == port->get_object_model())
		{
			delete item;
			break;
		}
	}
}

void WorkspaceWidget::future_connection_added(const RefPtr<PadTemplate>& src_tpl,
		const RefPtr<Element>& parent, const RefPtr<Pad>& sink)
{
	QNEPort* src_port = find_port(src_tpl, parent);
	QNEPort* sink_port = find_port(sink);

	if (!src_port || !sink_port)
		return;

	auto connection = new QNEConnection(0);
	scene->addItem(connection);
	connection->setPort1(src_port);
	connection->setPos1(src_port->scenePos());
	connection->setPort2(sink_port);
	connection->setPos2(sink_port->scenePos());
	connection->updatePath();
}

void WorkspaceWidget::future_connection_removed(const ConnectCommand::future_connection_pads& conn)
{
	QNEPort* src_port = find_port(conn.first.second, conn.first.first);
	QNEPort* sink_port = find_port(conn.second);

	if (!src_port || !sink_port)
		return;

	for (auto item : scene->items())
	{
		if (!item || item->type() != QNEConnection::Type)
			continue;

		QNEConnection* con = static_cast<QNEConnection*>(item);
		if (con->port1() && con->port2() &&
				(con->port1()->get_object_model() == src_port->get_object_model() &&
						con->port2()->get_object_model() == sink_port->get_object_model()))
		{
			delete item;
			break;
		}
	}
}
