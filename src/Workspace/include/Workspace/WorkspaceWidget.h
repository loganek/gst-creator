/*
 * WorkspaceWidget.h
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef WORKSPACEWIDGET_H_
#define WORKSPACEWIDGET_H_

#include "Commands.h"
#include "qnelibrary.h"
#include <QWidget>
#include <QMimeData>
#include <gstreamermm.h>

class WorkspaceWidget : public QWidget, public CommandListener
{
	Q_OBJECT
private:
	constexpr static const char* active_style_sheet = "QFrame{ border: 1px solid red; border-radius: 4px; padding: 2px;}";
	constexpr static const char* passive_style_sheet = "QFrame{ border: 1px solid black; border-radius: 4px; padding: 2px;}";

	CommandListener* controller;
	QGraphicsView* view;
	QGraphicsScene* scene;
	QNEConnection* current_connection;
	Glib::RefPtr<Gst::Pipeline> model;

	bool check_mime_data(const QMimeData* mime_data) const;
	QString get_new_name(const QString& name);
	QGraphicsItem* item_at(const QPointF &pos);
	bool eventFilter(QObject *o, QEvent *e);

	QNEPort* find_port(const Glib::RefPtr<Gst::Pad>& pad);
	QNEPort* find_port(const Glib::RefPtr<Gst::PadTemplate>& pad, const Glib::RefPtr<Gst::Element>& parent);
	QNEBlock* find_block(const Glib::RefPtr<Gst::Element>& element);

public:
	explicit WorkspaceWidget(const Glib::RefPtr<Gst::Pipeline>& model, QWidget* parent = 0);
	virtual ~WorkspaceWidget();

	void resizeEvent(QResizeEvent * event);

	void new_element_added(const Glib::RefPtr<Gst::Element>& element);
	void element_removed(const Glib::RefPtr<Gst::Element>& element);
	void pad_added(const Glib::RefPtr<Gst::Pad>& pad);
	void pad_linked(const Glib::RefPtr<Gst::Pad>& pad);
	void pad_removed(const Glib::RefPtr<Gst::Pad>& pad);
	void pad_unlinked(const Glib::RefPtr<Gst::Pad>& pad);
	void future_connection_added(const Glib::RefPtr<Gst::PadTemplate>& src_tpl,
			const Glib::RefPtr<Gst::Element>& parent, const Glib::RefPtr<Gst::Pad>& sink);
	void future_connection_removed(const ConnectCommand::future_connection_pads& conn);
	void set_controller(CommandListener* controller);

	QPointF get_block_location(const Glib::RefPtr<Gst::Element>& element);
	void set_block_location(const Glib::RefPtr<Gst::Element>& element, double x, double y);

Q_SIGNALS:
	void current_element_changed(const Glib::RefPtr<Gst::Element>& element);
};

#endif /* WORKSPACEWIDGET_H_ */
