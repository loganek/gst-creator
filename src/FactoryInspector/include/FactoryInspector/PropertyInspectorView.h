/*
 * PropertyInspectorView.h
 *
 *  Created on: 30 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef PROPERTYINSPECTORVIEW_H_
#define PROPERTYINSPECTORVIEW_H_

#include <QtWidgets>
#include <gstreamermm.h>

class PropertyInspectorView
{
private:
	QTreeWidget* tree;

	Glib::ustring get_flags_string(GParamFlags param_flags);
	bool is_numeric_property(GType type);
	void generate_numeric_property(QTreeWidgetItem* item, GValue value, GParamSpec* param);
	QTreeWidgetItem* generate_property_item(GParamSpec* param, GstElement* element);
public:
	PropertyInspectorView();
	QWidget* get_property_inspector_view(const Glib::RefPtr<Gst::ElementFactory>& factory);
};

#endif /* PROPERTYINSPECTORVIEW_H_ */
