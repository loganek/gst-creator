/*
 * WorkspaceWidget.h
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef WORKSPACEWIDGET_H_
#define WORKSPACEWIDGET_H_

#include "GstBlockInfo.h"
#include <QWidget>
#include <QMimeData>
#include <gstreamermm.h>

class WorkspaceWidget : public QWidget
{
	Q_OBJECT
private:
	constexpr static const char* active_style_sheet = "QFrame{ border: 1px solid red; border-radius: 4px; padding: 2px;}";
	constexpr static const char* passive_style_sheet = "QFrame{ border: 1px solid black; border-radius: 4px; padding: 2px;}";

	bool check_mime_data(const QMimeData* mime_data) const;
	void set_style_sheet(const char* style_sheet);
	QString get_new_name(const QString& name);

	QPixmap pixmap;
	GstBlockInfo* current_info;
	std::vector<GstBlockInfo*> blocks;

	Glib::RefPtr<Gst::Pipeline> model;

protected:
	void dragEnterEvent(QDragEnterEvent* event);
	void dragMoveEvent(QDragMoveEvent* event);
	void dragLeaveEvent(QDragLeaveEvent* event);
	void dropEvent(QDropEvent* event);
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent* event);

public:
	explicit WorkspaceWidget(const Glib::RefPtr<Gst::Pipeline>& model, QWidget* parent = 0);
	virtual ~WorkspaceWidget();

Q_SIGNALS:
	void current_element_changed(const Glib::RefPtr<Gst::Element>& element);
};


#endif /* WORKSPACEWIDGET_H_ */
