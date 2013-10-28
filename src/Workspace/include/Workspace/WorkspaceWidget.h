/*
 * WorkspaceWidget.h
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef WORKSPACEWIDGET_H_
#define WORKSPACEWIDGET_H_

#include <QWidget>
#include <QMimeData>

class WorkspaceWidget : public QWidget
{
	Q_OBJECT
private:
	constexpr static const char* active_style_sheet = "QFrame{ border: 1px solid red; border-radius: 4px; padding: 2px;}";
	constexpr static const char* passive_style_sheet = "QFrame{ border: 1px solid black; border-radius: 4px; padding: 2px;";

	bool check_mime_data(const QMimeData* mime_data) const;
	void set_style_sheet(const char* style_sheet);

protected:
	void dragEnterEvent(QDragEnterEvent* event);
	void dragMoveEvent(QDragMoveEvent* event);
	void dragLeaveEvent(QDragLeaveEvent* event);

public:
	explicit WorkspaceWidget(QWidget* parent = 0);
	virtual ~WorkspaceWidget();
};


#endif /* WORKSPACEWIDGET_H_ */
