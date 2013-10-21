/*
 * DraggedTreeView.h
 *
 *  Created on: 14 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef DRAGGEDTREEVIEW_H_
#define DRAGGEDTREEVIEW_H_

#include <QTreeView>
#include <QPoint>

class DraggedTreeView : public QTreeView
{
	Q_OBJECT
public:
	DraggedTreeView(QWidget *parent = 0);

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void startDrag(Qt::DropActions supportedActions);

private Q_SLOTS:
	void on_listView_clicked(const QModelIndex &index);
private:
	QString current_text;
	QPoint current_location;
};

#endif /* DRAGGEDTREEVIEW_H_ */
