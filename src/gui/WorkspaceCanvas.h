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

protected:
	void dragEnterEvent(QDragEnterEvent* event);
	void dragMoveEvent(QDragMoveEvent* event);
	void dropEvent(QDropEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void dragLeaveEvent(QDragLeaveEvent* event);
	void paintEvent(QPaintEvent* event);

	int find_piece(const QRect& block_rect) const;
public:
	explicit WorkspaceCanvas(QWidget* parent = 0);
	void clear_workspace();

};

#endif /* WORKSPACECANVAS_H_ */
