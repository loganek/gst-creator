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

protected:
	void dragEnterEvent(QDragEnterEvent* event);
	void dragMoveEvent(QDragMoveEvent* event);
	void dropEvent(QDropEvent* event);

public:
	explicit WorkspaceCanvas(QWidget* parent = 0);
	void clear_workspace();

};

#endif /* WORKSPACECANVAS_H_ */
