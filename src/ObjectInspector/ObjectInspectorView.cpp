/*
 * ObjectInspectorView.cpp
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#include "ObjectInspectorView.h"
#include <QDrag>
#include <QByteArray>
#include <QMimeData>
#include "../gui/blocks/GstBlock.h"
#include "ObjectInspectorModel.h"
#include "common.h"


ObjectInspectorView::ObjectInspectorView(QWidget* parent)
: QTreeView(parent)
{
	setSelectionMode(QAbstractItemView::SingleSelection);
	setDragEnabled(true);

	QObject::connect(this, &ObjectInspectorView::pressed, this, &ObjectInspectorView::on_object_inspector_clicked);
}

ObjectInspectorView::~ObjectInspectorView()
{}

void ObjectInspectorView::startDrag(Qt::DropActions supportedActions)
{
	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);

	GstBlock* block = new GstBlock(current_text.toUtf8().constData(), this);

	QPixmap pixmap = QPixmap::grabWidget(block);

	dataStream << pixmap << current_location << current_text;

	QMimeData *mimeData = new QMimeData;
	mimeData->setData(DRAG_DROP_FORMAT, itemData);

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setHotSpot(current_location);
	drag->setPixmap(QPixmap::grabWidget(block));
	delete block;

	int row = currentIndex().row();
	QModelIndex index = model()->index(row,0);

	if (drag->exec(Qt::MoveAction | Qt::CopyAction) == Qt::MoveAction)
	{
		model()->removeRow(index.row());
	}
}

void ObjectInspectorView::on_object_inspector_clicked(const QModelIndex &index)
{
	current_location = index.data(Qt::DisplayRole).toPoint();
	current_text = index.data(Qt::DisplayRole).toString();
}


