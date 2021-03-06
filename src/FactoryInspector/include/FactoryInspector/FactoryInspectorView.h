/*
 * FactoryInspectorView.h
 *
 *  Created on: 29 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef FACTORYINSPECTORVIEW_H_
#define FACTORYINSPECTORVIEW_H_

#include <QtWidgets>
#include <gstreamermm.h>

class FactoryInspectorView : public QWidget
{
	Q_OBJECT
private:
	void build_element_factory_view(const Glib::RefPtr<Gst::ElementFactory>& factory);
	void build_plugin_view(const Glib::RefPtr<Gst::Plugin>& plugin);
	void build_pad_templates_view(const Glib::RefPtr<Gst::ElementFactory>& factory);
	void build_pads_view(const Glib::RefPtr<Gst::ElementFactory>& factory);
	void build_properties_view(const Glib::RefPtr<Gst::ElementFactory>& factory);

public:
	FactoryInspectorView(bool is_factory, const QString& name, QWidget* parent = 0);
	virtual ~FactoryInspectorView();

	static const char* pad_direction_to_string(Gst::PadDirection direction);
	static const char* pad_presence_to_string(Gst::PadPresence presence);
	static QTreeWidgetItem* get_caps_view(const Glib::RefPtr<Gst::Caps>& caps);
};


#endif /* FACTORYINSPECTORVIEW_H_ */
