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
/*
void WorkspaceWidget::dropEvent(QDropEvent* event)
{
	if (!check_mime_data(event->mimeData()))
	{
		event->ignore();
		return;
	}

	if (line_drag)
	{
		line_drag = false;

		GstBlockInfo* my_info = nullptr;

		for (GstBlockInfo* info : blocks)
		{
			if (info->get_rect().contains(event->pos()))
			{
				my_info = info;
				break;
			}
		}

		this->setFocus();

		if (my_info == nullptr)
			return;

		QPoint cur_pt = event->pos() - QPoint(my_info->get_rect().x(), my_info->get_rect().y());
		GstPadWidget* pad = my_info->get_block()->find_pad(cur_pt);
		if (pad != nullptr && first_pad != nullptr && pad->get_pad()->get_direction() == Gst::PAD_SINK)
		{
			GstConnection* con = new GstConnection(first_pad, pad);
			if (con->may_exists())
			{
				ConnectCommand cmd(first_pad->get_pad(), pad->get_pad());
				cmd.run_command();
				connections.push_back(con);
				first_pad = nullptr;
			}
			else
				delete con;
		}

		return;
	}

	set_style_sheet(passive_style_sheet);

	bool present = false;
	QByteArray item_data = event->mimeData()->data(DRAG_DROP_FORMAT);
	QDataStream data_stream(&item_data, QIODevice::ReadOnly);
	QPoint location;
	int val;

	data_stream >> location >> val;
	GstBlock* element = reinterpret_cast<GstBlock*>(val); // fixme ugly code
	QRect rectangle = generate_rectangle(event->pos() - location, element);

	if (blocks.size() > 0)
	{
		for (auto block_info : blocks)
		{
			if (block_info->get_block()->get_model() == element->get_model())
			{
				present = true;
				current_info = block_info;
				current_info->set_rect(rectangle);
				current_info->set_location(location);
				Q_EMIT current_element_changed(current_info->get_block()->get_model());
				break;
			}
		}
	}

	if (blocks.size() == 0 || (blocks.size() > 0 && !present))
	{
		QString new_name = get_new_name(element->get_model()->get_name().c_str());
		element->get_model()->set_name(new_name.toUtf8().constData());
		GstBlockInfo* info = new GstBlockInfo(element, location, rectangle);
		blocks.push_back(info);
		AddCommand cmd(ObjectType::ELEMENT, model, element->get_model());
		cmd.run_command();
	}

	repaint();

	if (event->source() == this)
	{
		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
	else
		event->acceptProposedAction();
}
 */

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
					cmd = new DisconnectCommand(con->port1()->get_model(), con->port2()->get_model());
				else
					cmd = new DisconnectCommand(con->port2()->get_model(), con->port1()->get_model());
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
				QNEPort *port1 = conn->port1();
				QNEPort *port2 = (QNEPort*) item;

				if (port1->block() != port2->block() && port1->isOutput() != port2->isOutput() && !port1->isConnected(port2))
				{
					ConnectCommand* cmd;
					if (port1->isOutput())
						cmd = new ConnectCommand(port1->get_model(), port2->get_model());
					else
						cmd = new ConnectCommand(port2->get_model(), port1->get_model());

					cmd->run_command(this);
					delete cmd;

					conn->setPos2(port2->scenePos());
					conn->setPort2(port2);
					conn->updatePath();
					conn = 0;
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

	auto pad_iterator = element->iterate_pads();

	while (pad_iterator.next())
	{
		if (pad_iterator->get_direction() == Gst::PAD_SINK)
			b->addInputPort(*pad_iterator);
		else if (pad_iterator->get_direction() == Gst::PAD_SRC)
			b->addOutputPort(*pad_iterator);
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
					(con->port2()->get_model() == port->get_model() ||
							con->port1()->get_model() == port->get_model()))
			{
				delete item;
				break;
			}
		}
	}
}
