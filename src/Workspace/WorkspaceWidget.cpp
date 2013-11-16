/*
 * WorkspaceWidget.cpp
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#include "WorkspaceWidget.h"
#include "GstBlock.h"
#include "common.h"
#include <QtGui>
#include <QFrame>

WorkspaceWidget::WorkspaceWidget(const Glib::RefPtr<Gst::Pipeline>& model, QWidget* parent)
: QWidget(parent),
  model(model),
  current_info(nullptr)
{
	setAcceptDrops(true);
}

WorkspaceWidget::~WorkspaceWidget()
{
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
	if (!check_mime_data(event->mimeData()))
	{
		event->ignore();
		return;
	}

	event->setDropAction(Qt::MoveAction); // todo necessary?
	event->accept();

	repaint();
}

void WorkspaceWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
	set_style_sheet(passive_style_sheet);
	repaint();
}

QRect generate_rectangle(const QPoint& location)
{
	return QRect(location.x(), location.y(), 150, 50);
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

	set_style_sheet(passive_style_sheet);

	bool present = false;
	const QMimeData* mime = event->mimeData();
	QByteArray item_data = mime->data(DRAG_DROP_FORMAT);
	QDataStream data_stream(&item_data, QIODevice::ReadOnly);
	QPixmap pixmap;
	QPoint location;
	QString text;
	GstBlock* element;
	int val;
	data_stream >> pixmap >> location >> text >> val;
	element = reinterpret_cast<GstBlock*>(val); // fixme ugly code

	QRect rectangle = generate_rectangle(event->pos() - location);

	GstBlockInfo* info = nullptr;

	if (blocks.size() > 0)
	{
		for (auto block_info : blocks)
		{
			if (block_info->get_name() == text)
			{
				present = true;
				current_info = block_info;
				current_info->set_rect(rectangle);
				current_info->set_location(location);
				break;
			}
		}
	}

	if (blocks.size() == 0 || (blocks.size() > 0 && !present))
	{
		QString new_name = get_new_name(text).toUtf8().constData();
		element->get_model()->set_name(new_name.toUtf8().constData());
		info = new GstBlockInfo(QPixmap::grabWidget(element), location, new_name, rectangle);
		blocks.push_back(info);
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

	for (auto info : blocks)
	{
		painter.drawPixmap(info->get_rect(), info->get_pixmap());
	}
}

void WorkspaceWidget::mousePressEvent(QMouseEvent* event)
{
	QRect rectangle = generate_rectangle(event->pos());

	current_info = nullptr;

	for (GstBlockInfo* info : blocks)
	{
		if (info->get_rect().contains(rectangle.x(),rectangle.y()))
			current_info = info;
	}

	this->setFocus();

	if (current_info == nullptr)
		return;

	QPoint position(current_info->get_rect().x(), current_info->get_rect().y());

	QPoint location = event->pos() - position;
	QPixmap pixmap = current_info->get_pixmap();
	QString text = current_info->get_name();

	QByteArray item_data;
	QDataStream dataStream(&item_data, QIODevice::WriteOnly);

	dataStream << pixmap << location << text;

	QMimeData *mime_data = new QMimeData;
	mime_data->setData(DRAG_DROP_FORMAT, item_data);

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mime_data);
	drag->setHotSpot(location);
	drag->setPixmap(pixmap);

	drag->exec(Qt::MoveAction | Qt::CopyAction);
	repaint();
}
