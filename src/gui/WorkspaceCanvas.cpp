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

	repaint();
}

void WorkspaceCanvas::dropEvent(QDropEvent* event)
{
	if (!event->mimeData()->hasFormat("application/x-DraggedTreeView-DragAndDrop"))
	{
		event->ignore();
		return;
	}

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
	QRect rectangle(start.x(), start.y(), 200, 50);

	GstBlockInfo* info = nullptr;

	if (GstBlockInfo::get_blocks().size() == 0)
	{
		info = new GstBlockInfo(pixmap, location, text, rectangle);

		GstBlockInfo::get_blocks().push_back(info);
	}
	else
	{
		for (auto block_info : GstBlockInfo::get_blocks())
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

		if (!present)
		{
			info = new GstBlockInfo(pixmap, location, text, rectangle);

			GstBlockInfo::get_blocks().push_back(info);
		}
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
	repaint();
}

void WorkspaceCanvas::mousePressEvent(QMouseEvent* event)
{
	QRect rectangle(event->pos().x(), event->pos().y(), 200, 50);
	int found = find_piece(rectangle);
	this->setFocus();

	if (found == -1)
		return;


	current_info = GstBlockInfo::get_blocks().at(found);

	QPoint position(current_info->get_rect().x(),current_info->get_rect().y());

	QPoint location = event->pos() - position;
	QPixmap pixmap = current_info->get_pixmap();
	QString text = current_info->get_name();

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);

	dataStream << pixmap << location << text;

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-DraggedTreeView-DragAndDrop", itemData);

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setHotSpot(location);
	drag->setPixmap(pixmap);

	repaint();

	if (drag->exec(Qt::MoveAction | Qt::CopyAction) == Qt::MoveAction)
	{
		repaint();
	}
}


int WorkspaceCanvas::find_piece(const QRect& block_rect) const
{
	int i=0;
	for (GstBlockInfo* info : GstBlockInfo::get_blocks())
	{
		if (info->get_rect().contains(block_rect.x(),block_rect.y()))
			return i;

		i++;
	}

	return -1;
}

void WorkspaceCanvas::paintEvent(QPaintEvent* event)
{
	QPainter painter;
	painter.begin(this);

	for (auto info : GstBlockInfo::get_blocks())
	{
		painter.drawPixmap(info->get_rect(), info->get_pixmap());
	}
}
