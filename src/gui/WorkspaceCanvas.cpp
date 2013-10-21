/*
 * WorkspaceCanvas.cpp
 *
 *  Created on: 16 pa? 2013
 *      Author: Marcin Kolny
 */

#include "WorkspaceCanvas.h"
#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>

WorkspaceCanvas::WorkspaceCanvas(QWidget* parent)
: QWidget(parent),
  current_info(nullptr)
{
	setAcceptDrops(true);
}

bool WorkspaceCanvas::check_mime_data(const QMimeData* mime_data) const
{
	return mime_data->hasFormat(mime_drag_drop_format);
}

void WorkspaceCanvas::set_style_sheet(const char* style_sheet)
{
	QFrame* frame = static_cast<QFrame*> (this->parent());
	frame->setStyleSheet(style_sheet);
}

GstBlockInfo* WorkspaceCanvas::find_piece(const QRect& block_rect) const
{
	for (GstBlockInfo* info : blocks)
	{
		if (info->get_rect().contains(block_rect.x(),block_rect.y()))
			return info;
	}

	return nullptr;
}

QRect WorkspaceCanvas::generate_rectangle(const QPoint& location) const
{
	return QRect(location.x(), location.y(), 150,50);
}

void WorkspaceCanvas::dragEnterEvent(QDragEnterEvent* event)
{
	if (!check_mime_data(event->mimeData()))
	{
		event->ignore();
		return;
	}

	set_style_sheet(active_style_sheet);
	event->accept();
}

void WorkspaceCanvas::dragMoveEvent(QDragMoveEvent* event)
{
	if (!check_mime_data(event->mimeData()))
	{
		event->ignore();
		return;
	}

	event->setDropAction(Qt::MoveAction);
	event->accept();

	repaint();
}

void WorkspaceCanvas::dropEvent(QDropEvent* event)
{
	if (!check_mime_data(event->mimeData()))
	{
		event->ignore();
		return;
	}

	set_style_sheet(passive_style_sheet);

	bool present = false;
	const QMimeData* mime = event->mimeData();
	QByteArray item_data = mime->data(mime_drag_drop_format);
	QDataStream data_stream(&item_data, QIODevice::ReadOnly);
	QPixmap pixmap;
	QPoint location;
	QString text;
	data_stream >> pixmap >> location >> text;
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
		info = new GstBlockInfo(pixmap, location, text, rectangle);
		blocks.push_back(info);
	}

	repaint();


	if (event->source() == this)
	{
		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
	else
	{
		event->acceptProposedAction();
	}
}

void WorkspaceCanvas::dragLeaveEvent(QDragLeaveEvent* event)
{
	set_style_sheet(passive_style_sheet);
	repaint();
}

void WorkspaceCanvas::mousePressEvent(QMouseEvent* event)
{
	QRect rectangle = generate_rectangle(event->pos());
	current_info = find_piece(rectangle);
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
	mime_data->setData(mime_drag_drop_format, item_data);

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mime_data);
	drag->setHotSpot(location);
	drag->setPixmap(pixmap);

	drag->exec(Qt::MoveAction | Qt::CopyAction);
	repaint();
}

void WorkspaceCanvas::paintEvent(QPaintEvent* event)
{
	QPainter painter;
	painter.begin(this);

	for (auto info : blocks)
	{
		painter.drawPixmap(info->get_rect(), info->get_pixmap());
	}
}

