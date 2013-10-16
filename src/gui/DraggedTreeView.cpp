/*
 * DraggedTreeView.cpp
 *
 *  Created on: 14 pa? 2013
 *      Author: loganek
 */

#include "DraggedTreeView.h"
#include <QDrag>
#include <QDragEnterEvent>
#include <QLineEdit>
#include <QByteArray>
#include <QMimeData>
#include "GstBlock.h"


DraggedTreeView::DraggedTreeView(QWidget* parent)
: QTreeView(parent)
{
	setSelectionMode(QAbstractItemView::SingleSelection);
	setDragEnabled(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
	setDragDropMode(QAbstractItemView::InternalMove);

	connect(this, SIGNAL(pressed(QModelIndex)), this, SLOT(on_listView_clicked(QModelIndex)));
}

void DraggedTreeView::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

void DraggedTreeView::dragMoveEvent(QDragMoveEvent *event)
{
	event->setDropAction(Qt::MoveAction);
	event->accept();
}

void DraggedTreeView::startDrag(Qt::DropActions supportedActions)
{
	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);

	GstBlock* block = new GstBlock(this);
	block->set_name(current_text);

	QPixmap pixmap = QPixmap::grabWidget(block);

	dataStream << pixmap << current_location << current_text;

	QMimeData *mimeData = new QMimeData;
	mimeData->setData("application/x-DraggedTreeView-DragAndDrop", itemData);

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setHotSpot(current_location);
	drag->setPixmap(QPixmap::grabWidget(block));

	int row = currentIndex().row();
	QModelIndex index = model()->index(row,0);

	delete block;

	if (drag->exec(Qt::MoveAction | Qt::CopyAction) == Qt::MoveAction)
	{
		model()->removeRow(index.row());
	}
}

void DraggedTreeView::on_listView_clicked(const QModelIndex &index)
{
	current_location = index.data(Qt::DisplayRole).toPoint();
	current_text = index.data(Qt::DisplayRole).toString();
}
