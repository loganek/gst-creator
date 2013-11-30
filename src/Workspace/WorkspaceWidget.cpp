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
  current_info(nullptr),
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


	QNEBlock *b = new QNEBlock(0);
	scene->addItem(b);
	b->addPort("test", 0, QNEPort::NamePort);
	b->addPort("TestBlock", 0, QNEPort::TypePort);
	b->addInputPort("in1");
	b->addInputPort("in2");
	b->addInputPort("in3");
	b->addOutputPort("out1");
	b->addOutputPort("out2");
	b->addOutputPort("out3");
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

void WorkspaceWidget::set_style_sheet(const char* style_sheet)
{
	QFrame* frame = static_cast<QFrame*> (this->parent());
	frame->setStyleSheet(style_sheet);
}

void WorkspaceWidget::dragEnterEvent(QDragEnterEvent* event)
{
	if (!check_mime_data(event->mimeData()))
	{
		event->ignore();
		return;
	}

	set_style_sheet(active_style_sheet);
	event->accept();
}

void WorkspaceWidget::dragMoveEvent(QDragMoveEvent* event)
{
	if (line_drag)
	{
		greenline = false;
		curr_line_pos = event->pos();

		GstBlockInfo* my_info = nullptr;

		for (GstBlockInfo* info : blocks)
		{
			if (info->get_rect().contains(event->pos()))
			{
				my_info = info;
				break;
			}
		}

		event->accept();

		if (my_info != nullptr)
		{
			QPoint cur_pt = event->pos() - QPoint(my_info->get_rect().x(), my_info->get_rect().y());
			GstPadWidget* pad_w = my_info->get_block()->find_pad(cur_pt);

			if (pad_w != nullptr && pad_w->get_pad() != first_pad->get_pad())
			{
				GstConnection con (first_pad, pad_w);
				if (con.may_exists())
					greenline = true;
			}
		}
		repaint();

		return;
	}

	if (!check_mime_data(event->mimeData()))
	{
		event->ignore();
		return;
	}

	event->accept();

	repaint();
}

void WorkspaceWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
	set_style_sheet(passive_style_sheet);
	repaint();
}

QRect generate_rectangle(const QPoint& location, GstBlock* block)
{
	return QRect(location.x(), location.y(), block->get_width(), block->get_height());
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


void WorkspaceWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter;
	painter.begin(this);

	if (line_drag)
		GstConnection::draw_arrow(painter, first_pad->get_absolute_position(), curr_line_pos,
				greenline ? ConnectState::OK_CONNECTION : ConnectState::BAD_CONNECTION);

	for (auto connection : connections)
		connection->draw_arrow(painter);

	for (auto info : blocks)
		painter.drawPixmap(info->get_rect(), info->get_pixmap());
}

void WorkspaceWidget::mousePressEvent(QMouseEvent* event)
{
	current_info = nullptr;

	for (GstBlockInfo* info : blocks)
	{
		if (info->get_rect().contains(event->pos()))
			current_info = info;
	}

	this->setFocus();

	if (current_info == nullptr)
		return;

	GstBlock* block = current_info->get_block();
	GstPadWidget* pad = block->find_pad(event->pos() - QPoint(current_info->get_rect().x(), current_info->get_rect().y()));



	QPoint position(current_info->get_rect().x(), current_info->get_rect().y());
	QPoint location = event->pos() - position;

	QByteArray item_data;
	QDataStream data_stream(&item_data, QIODevice::WriteOnly);

	data_stream << location << reinterpret_cast<int>(current_info->get_block());

	QMimeData* mime_data = new QMimeData;
	mime_data->setData(DRAG_DROP_FORMAT, item_data);

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mime_data);
	if (pad != nullptr && pad->get_pad()->get_direction() == Gst::PAD_SRC)
	{
		first_pad = pad;
		line_drag = true;
	}
	else{
		drag->setHotSpot(location);
		drag->setPixmap(current_info->get_pixmap());
	}
	drag->exec(Qt::MoveAction | Qt::CopyAction);

	repaint();

	Q_EMIT current_element_changed(current_info->get_block()->get_model());
}

GstBlockInfo* WorkspaceWidget::find_block(const std::string& name)
{
	for (auto block: blocks)
	{
		if (block->get_name() == name.c_str())
			return block;
	}

	return nullptr;
}

void WorkspaceWidget::model_changed(std::shared_ptr<Command> cmd)
{
	/*switch (cmd->get_type())
	{
	case CommandType::ADD:
	{
		Glib::RefPtr<Gst::Object> ob = ob.cast_static(std::static_pointer_cast<AddCommand>(cmd)->get_object());
		GstBlock* block = new GstBlock(Glib::RefPtr<Gst::Element>::cast_static(ob));
		GstBlockInfo* info = new GstBlockInfo(block,
				QPoint(10, 10), QRect(0, 0, block->get_width(), block->get_height()));
		blocks.push_back(info);
		repaint();
	}
	break;
	case CommandType::PROPERTY:
		break;
	case CommandType::CONNECT:
	{
		std::shared_ptr<ConnectCommand> concmd =
				std::static_pointer_cast<ConnectCommand>(cmd);
		GstConnection* con;

		if (concmd->get_type() == ObjectType::PAD)
		{
			Glib::RefPtr<Gst::Pad> src_pad = src_pad.cast_static(concmd->get_src());
			Glib::RefPtr<Gst::Pad> sink_pad = sink_pad.cast_static(concmd->get_dst());

			GstBlockInfo* src_block = find_block(src_pad->get_parent()->get_name().c_str());
			GstBlockInfo* sink_block = find_block(sink_pad->get_parent()->get_name().c_str());

			if (src_block == nullptr || sink_block == nullptr)
				return;

			GstPadWidget* src_pad_widget =
					src_block->get_block()->find_pad(concmd->get_src()->get_name().c_str());
			GstPadWidget* sink_pad_widget =
					sink_block->get_block()->find_pad(concmd->get_dst()->get_name().c_str());

			if (sink_pad_widget == nullptr || src_pad_widget == nullptr)
							return;

			con = new GstConnection(src_pad_widget, sink_pad_widget);
		}
		else
			return;
		connections.push_back(con);
		repaint();
		break;
	}
	}*/
}


QGraphicsItem* WorkspaceWidget::item_at(const QPointF &pos)
{
	QList<QGraphicsItem*> items = scene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

	Q_FOREACH(QGraphicsItem *item, items)
	if (item->type() > QGraphicsItem::UserType)
		return item;

	return nullptr;
}

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
			} else if (item && item->type() == QNEBlock::Type)
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
			if (item && (item->type() == QNEConnection::Type || item->type() == QNEBlock::Type))
				delete item;
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
	}
	return QObject::eventFilter(o, e);
}


