/*
 * WorkspaceWidget.h
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef WORKSPACEWIDGET_H_
#define WORKSPACEWIDGET_H_

#include "GstBlockInfo.h"
#include "Commands/Command.h"
#include "GstConnection.h"
#include "GstPadWidget.h"
#include "qnelibrary.h"
#include "Commands/CommandListener.h"
#include <QWidget>
#include <QMimeData>
#include <gstreamermm.h>
#include <vector>

class WorkspaceWidget : public QWidget, public CommandListener
{
	Q_OBJECT
private:
	constexpr static const char* active_style_sheet = "QFrame{ border: 1px solid red; border-radius: 4px; padding: 2px;}";
	constexpr static const char* passive_style_sheet = "QFrame{ border: 1px solid black; border-radius: 4px; padding: 2px;}";

	bool greenline;
	bool line_drag;
	GstPadWidget* first_pad;
	GstPadWidget* second_pad;

	bool check_mime_data(const QMimeData* mime_data) const;
	QString get_new_name(const QString& name);

	QGraphicsItem* item_at(const QPointF &pos);
	QGraphicsView* view;

	Glib::RefPtr<Gst::Pipeline> model;

	QGraphicsScene* scene;
	QNEConnection* conn;
	bool eventFilter(QObject *o, QEvent *e);

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

Q_SIGNALS:
	void current_element_changed(const Glib::RefPtr<Gst::Element>& element);
};


#endif /* WORKSPACEWIDGET_H_ */
