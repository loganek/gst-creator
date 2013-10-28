/*
 * WorkspaceWidget.cpp
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#include "WorkspaceWidget.h"
#include "common.h"
#include <QtGui>
#include <QFrame>

WorkspaceWidget::WorkspaceWidget(QWidget* parent)
: QWidget(parent)
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

	event->accept();

	repaint();
}

void WorkspaceWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
	set_style_sheet(passive_style_sheet);
	repaint();
}
