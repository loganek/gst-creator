/*
 * WorkspaceCanvas.h
 *
 *  Created on: 16 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef WORKSPACECANVAS_H_
#define WORKSPACECANVAS_H_

#include "GstBlockInfo.h"

#include <QWidget>
#include <vector>

class WorkspaceCanvas : public QWidget
{
	Q_OBJECT
private:
	GstBlockInfo* current_info;
	std::vector<GstBlockInfo*> blocks;

	constexpr static const char* mime_drag_drop_format = "application/x-DraggedTreeView-DragAndDrop";
	constexpr static const char* active_style_sheet = "QFrame{ border: 1px solid red; border-radius: 4px; padding: 2px;}";
	constexpr static const char* passive_style_sheet = "QFrame{ border: 1px solid black; border-radius: 4px; padding: 2px;";

	bool check_mime_data(const QMimeData* mime_data) const;
	GstBlockInfo* find_piece(const QRect& block_rect) const;
	QRect generate_rectangle(const QPoint& location) const;
	void set_style_sheet(const char* style_sheet);
	QString get_new_name(const QString& name);
protected:
	void dragEnterEvent(QDragEnterEvent* event);
	void dragMoveEvent(QDragMoveEvent* event);
	void dropEvent(QDropEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void dragLeaveEvent(QDragLeaveEvent* event);
	void paintEvent(QPaintEvent* event);
public:
	explicit WorkspaceCanvas(QWidget* parent = 0);
};

#endif /* WORKSPACECANVAS_H_ */
