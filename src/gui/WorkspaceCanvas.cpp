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
: QWidget(parent)
{
	setAcceptDrops(true);
}

void WorkspaceCanvas::clear_workspace()
{

}

void WorkspaceCanvas::dragEnterEvent(QDragEnterEvent* event)
{
	if (!event->mimeData()->hasFormat("application/x-DraggedTreeView-DragAndDrop"))
	{
		event->ignore();
		return;
	}

	QFrame* frame = static_cast<QFrame*>(this->parent());
	QString style_sheet = "QFrame{ border: 1px solid red; border-radius: 4px; padding: 2px;}";
	frame->setStyleSheet(style_sheet);
	event->accept();
}

void WorkspaceCanvas::dragMoveEvent(QDragMoveEvent* event)
{
	if (!event->mimeData()->hasFormat("application/x-DraggedTreeView-DragAndDrop"))
	{
		event->ignore();
		return;
	}

	event->setDropAction(Qt::MoveAction);
	event->accept();
}

void WorkspaceCanvas::dropEvent(QDropEvent* event)
{
	if (!event->mimeData()->hasFormat("application/x-DraggedTreeView-DragAndDrop"))
	{
		event->ignore();
		return;
	}

	clear_workspace();

	QFrame* frame = static_cast<QFrame*>(this->parent());

	bool present = false;
	const QMimeData* mime = event->mimeData();
	QByteArray item_data = mime->data("application/x-DraggedTreeView-DragAndDrop");
	QDataStream data_stream(&item_data, QIODevice::ReadOnly);
	QPixmap pixmap;
	QPoint location;
	QString text;
	data_stream >> pixmap >> location >> text;
	QPoint start = event->pos() - location;
	QRect rectangle(start.x(), start.y(), 300, 100);

	GstBlockInfo* info = nullptr;

	if (blocks.size() == 0)
	{
		info = new GstBlockInfo(pixmap, location, text, rectangle);

		blocks.push_back(info);
	}
	else
	{
		for (auto block_info : blocks)
		{
			if (block_info->get_name() == text)
			{
				present = true;
				current_info = block_info;
				break;
			}
		}

		if (!present)
		{
			info = new GstBlockInfo(pixmap, location, text, rectangle);

			blocks.push_back(info);
		}
	}

	if (event->source() == this)
	{
		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
	else
	{
		event->acceptProposedAction();
	}
	repaint();
}

void WorkspaceCanvas::dragLeaveEvent(QDragLeaveEvent* event)
{
	clear_workspace();
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
