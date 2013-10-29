/*
 * ObjectInspectorView.h
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef OBJECTINSPECTORVIEW_H_
#define OBJECTINSPECTORVIEW_H_

#include <QTreeView>
#include <QPoint>

class ObjectInspectorView : public QTreeView
{
	Q_OBJECT

private Q_SLOTS:
	void on_object_inspector_clicked(const QModelIndex &index);
	void on_object_inspector_double_clicked(const QModelIndex &index);

private:
	QString current_text;
	QPoint current_location;

protected:
	void startDrag(Qt::DropActions supportedActions);

public:
	ObjectInspectorView(QWidget *parent = 0);
	virtual ~ObjectInspectorView();
};

#endif /* OBJECTINSPECTORVIEW_H_ */
