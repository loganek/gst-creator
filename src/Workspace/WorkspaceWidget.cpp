/*
 * WorkspaceWidget.cpp
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#include "WorkspaceWidget.h"
#include "GstBlock.h"
#include "common.h"
#include "Commands.h"
#include "utils/GstUtils.h"
#include <QtGui>
#include <QFrame>

WorkspaceWidget::WorkspaceWidget(const Glib::RefPtr<Gst::Pipeline>& model, QWidget* parent)
: QWidget(parent),
  model(model),
  first_pad(nullptr),
  second_pad(nullptr),
  line_drag(false),
  greenline(false),
  conn(nullptr)
{
	setAcceptDrops(true);
	scene = new QGraphicsScene();
	scene->installEventFilter(this);
	view = new QGraphicsView(scene, this);
	view->setRenderHint(QPainter::Antialiasing, true);
	this->installEventFilter(this);
}

void WorkspaceWidget::resizeEvent(QResizeEvent * event)
{
	view->resize(event->size());
}

WorkspaceWidget::~WorkspaceWidget()
{
	delete view;
	delete scene;
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
			if (item && item->type() == QNEPort::Type)
			{
				conn = new QNEConnection(0);
				scene->addItem(conn);
				conn->setPort1((QNEPort*) item);
				conn->setPos1(item->scenePos());
				conn->setPos2(me->scenePos());
				conn->updatePath();

				return true;
			}
			else if (item && item->type() == QNEBlock::Type)
			{
				/* if (selBlock)
					selBlock->setSelected(); */
				// selBlock = (QNEBlock*) item;
			}
			break;
		}
		case Qt::RightButton:
		{
			QGraphicsItem *item = item_at(me->scenePos());
			if (item && (item->type() == QNEConnection::Type))
			{
				QNEConnection* con = static_cast<QNEConnection*>(item);
				DisconnectCommand* cmd;
				if (con->port1()->isOutput())
					cmd = new DisconnectCommand(con->port1()->get_object_model(), con->port2()->get_object_model());
				else
					cmd = new DisconnectCommand(con->port2()->get_object_model(), con->port1()->get_object_model());
				cmd->run_command(this);
				delete cmd;
			}

			else if (item && (item->type() == QNEBlock::Type))
			{
				QNEBlock* block = static_cast<QNEBlock*>(item);
				RemoveCommand cmd(ObjectType::ELEMENT, block->get_model());
				cmd.run_command(this);
			}
			// if (selBlock == (QNEBlock*) item)
			// selBlock = 0;
			break;
		}
		}
	}
	case QEvent::GraphicsSceneMouseMove:
	{
		if (conn)
		{
			conn->setPos2(me->scenePos());
			conn->updatePath();
			QGraphicsItem *item = item_at(me->scenePos());
			if (item && item != conn->port1() && item->type() == QNEPort::Type)
			{
				QNEPort *src_port = (conn->port1()->isOutput()) ? conn->port1() : (QNEPort*) item;
				QNEPort *sink_port = (!conn->port1()->isOutput()) ? conn->port1() : (QNEPort*) item;

				if (src_port->can_link(sink_port))
					conn->connectColor(1);
				else
					conn->connectColor(0);

			}
			else
				conn->connectColor(2);

			return true;
		}

		break;
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
		if (conn && me->button() == Qt::LeftButton)
		{
			QGraphicsItem *item = item_at(me->scenePos());
			if (item && item->type() == QNEPort::Type)
			{
				QNEPort *src_port = (conn->port1()->isOutput()) ? conn->port1() : (QNEPort*) item;
				QNEPort *sink_port = (!conn->port1()->isOutput()) ? conn->port1() : (QNEPort*) item;

				delete conn;
				conn = 0;

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

					Glib::RefPtr<Gst::Pad> src_pad = src_pad.cast_static(src_port->get_object_model());
					Glib::RefPtr<Gst::Pad> sink_pad = sink_pad.cast_static(sink_port->get_object_model());
					if (src_port->is_template_model() && Glib::RefPtr<Gst::PadTemplate>::cast_static(src_port->get_object_model())->get_presence() == Gst::PAD_REQUEST)
					{
						auto tpl = Glib::RefPtr<Gst::PadTemplate>::cast_static(src_port->get_object_model());
						AddCommand add_cmd(ObjectType::PAD, src_port->block()->get_model(), tpl);
						src_pad = src_pad.cast_static(add_cmd.run_command_ret(this));
					}
					else if (sink_port->is_template_model() && Glib::RefPtr<Gst::PadTemplate>::cast_static(sink_port->get_object_model())->get_presence() == Gst::PAD_REQUEST)
					{
						auto tpl = Glib::RefPtr<Gst::PadTemplate>::cast_static(sink_port->get_object_model());
						AddCommand add_cmd(ObjectType::PAD, sink_port->block()->get_model(), tpl);
						sink_pad = sink_pad.cast_static(add_cmd.run_command_ret(this));
					}
					if (src_port->is_template_model() && Glib::RefPtr<Gst::PadTemplate>::cast_static(src_port->get_object_model())->get_presence() == Gst::PAD_SOMETIMES)
					{
						auto pad_parent = src_port->block()->get_model();
						auto tpl = Glib::RefPtr<Gst::PadTemplate>::cast_static(src_port->get_object_model());
						ConnectCommand con_cmd(tpl, pad_parent, sink_pad);
						con_cmd.run_command(this);
						return true;
					}
					ConnectCommand cmd(src_pad, sink_pad);
					cmd.run_command(this);


					return true;
				}
			}

			delete conn;
			conn = 0;
			return true;
		}
		break;
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
		QPoint location;
		QString nn;
		data_stream >> location >> nn;

		auto element = Gst::ElementFactory::create_element(nn.toUtf8().constData());
		QString new_name = get_new_name(element->get_name().c_str());
		element->set_name(new_name.toUtf8().constData());
		AddCommand cmd(ObjectType::ELEMENT, model, element);
		cmd.run_command(this);
		last_point = me->scenePos();
		return true;
	}
	}
	return QObject::eventFilter(o, e);
}

void WorkspaceWidget::new_element_added(const Glib::RefPtr<Gst::Element>& element)
{
	QNEBlock *b = new QNEBlock(element, 0);
	scene->addItem(b);
	b->addPort(element, 0, QNEPort::NamePort);
	if (!GstUtils::is_src_element(element))
		b->addInputPort(Glib::RefPtr<Gst::Object>());
	if (!GstUtils::is_sink_element(element))
		b->addOutputPort(Glib::RefPtr<Gst::Object>());

	auto pad_iterator = element->iterate_pads();

	while (pad_iterator.next())
	{
		if (pad_iterator->get_direction() == Gst::PAD_SINK)
			b->addInputPort(*pad_iterator);
		else if (pad_iterator->get_direction() == Gst::PAD_SRC)
			b->addOutputPort(*pad_iterator);
	}

	std::vector<Gst::StaticPadTemplate> templates = element->get_factory()->get_static_pad_templates();

	for (auto tpl : templates)
	{
		if (tpl.get_presence() == Gst::PAD_SOMETIMES || tpl.get_presence() == Gst::PAD_REQUEST)
			b->addPort(element->get_pad_template(tpl.get_name_template()), tpl.get_direction() == Gst::PAD_SRC);
	}

	b->setPos(last_point);
}

void WorkspaceWidget::element_removed(const Glib::RefPtr<Gst::Element>& element)
{
	QNEBlock *b = find_block(element);
	delete b;
}

QNEBlock* WorkspaceWidget::find_block(const Glib::RefPtr<Gst::Element>& element)
{
	QList<QGraphicsItem*> items = scene->items();

	for (auto item : items)
	{
		if (item && item->type() == QNEBlock::Type)
		{
			QNEBlock* block = static_cast<QNEBlock*>(item);

			if (block && block->get_model() == element)
				return block;
		}
	}

	return nullptr;
}

void WorkspaceWidget::pad_added(const Glib::RefPtr<Gst::Pad>& pad)
{
	QNEBlock* block = find_block(pad->get_parent_element());

	if (block == nullptr)
		return;

	if (pad->get_direction() == Gst::PAD_SINK)
		block->addInputPort(pad);
	else if (pad->get_direction() == Gst::PAD_SRC)
		block->addOutputPort(pad);
}

void WorkspaceWidget::pad_linked(const Glib::RefPtr<Gst::Pad>& pad)
{
	if (pad->get_direction() == Gst::PAD_SINK)
		return;
	QNEBlock* first_block = find_block(pad->get_peer()->get_parent_element()),
			*second_block = find_block(pad->get_parent_element());
	QNEPort* first_port = nullptr, *second_port = nullptr;

	if (first_block == nullptr || second_block == nullptr)
		return;

	first_port = first_block->find_port(pad->get_peer());
	second_port = second_block->find_port(pad);

	if (first_port == nullptr || second_port == nullptr)
		return;

	QNEConnection* connection = new QNEConnection(0);
	scene->addItem(connection);
	connection->setPos2(second_port->scenePos());
	connection->setPort2(second_port);
	connection->setPos1(first_port->scenePos());
	connection->setPort1(first_port);
	connection->updatePath();
}

void WorkspaceWidget::pad_removed(const Glib::RefPtr<Gst::Pad>& pad)
{
	QNEBlock* block = find_block(pad->get_parent_element());

	if (block == nullptr)
		return;

	QNEPort* port = block->find_port(pad);

	if (port == nullptr)
		return;

	delete port;
}

void WorkspaceWidget::pad_unlinked(const Glib::RefPtr<Gst::Pad>& pad)
{
	if (pad->get_direction() == Gst::PAD_SINK)
		return;
	QNEBlock* block = find_block(pad->get_parent_element());
	QNEPort* port = nullptr;

	if (block == nullptr)
		return;

	port = block->find_port(pad);

	if (port == nullptr)
		return;

	QList<QGraphicsItem*> items = scene->items();

	for (auto item : items)
	{
		if (item && item->type() == QNEConnection::Type)
		{
			QNEConnection* con = static_cast<QNEConnection*>(item);
			if (con->port1() && con->port2() &&
					(con->port2()->get_object_model() == port->get_object_model() ||
							con->port1()->get_object_model() == port->get_object_model()))
			{
				delete item;
				break;
			}
		}
	}
}

void WorkspaceWidget::future_connection_added(const Glib::RefPtr<Gst::PadTemplate>& src_tpl,
		const Glib::RefPtr<Gst::Element>& parent, const Glib::RefPtr<Gst::Pad>& sink)
{
	QNEBlock* src_block = find_block(parent);
	QNEBlock* sink_block = find_block(sink->get_parent_element());

	if (!src_block || !sink_block)
		return;

	QNEPort* src_port = src_block->find_port(src_tpl);
	QNEPort* sink_port = sink_block->find_port(sink);

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
	QNEBlock* src_block = find_block(conn.first.first);
	QNEBlock* sink_block = find_block(conn.second->get_parent_element());

	if (!src_block || !sink_block)
		return;

	QNEPort* src_port = src_block->find_port(conn.first.second);
	QNEPort* sink_port = sink_block->find_port(conn.second);

	if (!src_port || !sink_port)
		return;

	QList<QGraphicsItem*> items = scene->items();

	for (auto item : items)
	{
		if (item && item->type() == QNEConnection::Type)
		{
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
}
