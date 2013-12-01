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
#include <QMainWindow>
#include "Workspace/GstBlock.h"
#include "ObjectInspectorModel.h"
#include "common.h"
#include "FactoryInspector/FactoryInspectorView.h"


ObjectInspectorView::ObjectInspectorView(QWidget* parent)
: QTreeView(parent)
{
	setSelectionMode(QAbstractItemView::SingleSelection);
	setDragEnabled(true);

	QObject::connect(this, &ObjectInspectorView::pressed, this, &ObjectInspectorView::on_object_inspector_clicked);
	QObject::connect(this, &ObjectInspectorView::doubleClicked, this, &ObjectInspectorView::on_object_inspector_double_clicked);
}

ObjectInspectorView::~ObjectInspectorView()
{}

void ObjectInspectorView::startDrag(Qt::DropActions supportedActions)
{
	QByteArray itemData;
	QDataStream data_stream(&itemData, QIODevice::WriteOnly);

	auto element = Gst::ElementFactory::create_element(current_text.toUtf8().constData());
	element->reference();
	GstBlock* block = new GstBlock(element, this);

	data_stream << current_location << current_text;

	QMimeData *mime_data = new QMimeData;
	mime_data->setData(DRAG_DROP_FORMAT, itemData);

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mime_data);
	drag->setHotSpot(current_location);
	drag->setPixmap(block->grab());
	//delete block;

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

void ObjectInspectorView::on_object_inspector_double_clicked(const QModelIndex &index)
{
	QString factory_name = index.data(Qt::DisplayRole).toString();
	QDialog dialog;
	dialog.setWindowTitle("Inspect " + factory_name);
	QHBoxLayout* lay = new QHBoxLayout();
	dialog.setLayout(lay);
	dialog.layout()->addWidget(new FactoryInspectorView(factory_name));
	dialog.exec();
}


