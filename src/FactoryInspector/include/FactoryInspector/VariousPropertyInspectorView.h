/*
 * VariousPropertyInspectorView.h
 *
 *  Created on: 30 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef VARIOUSPROPERTYINSPECTORVIEW_H_
#define VARIOUSPROPERTYINSPECTORVIEW_H_

#include <QtWidgets>
#include <gstreamermm.h>

class VariousPropertyInspectorView
{
	QTreeWidgetItem* item;
	GValue value;
	GParamSpec* param;

	static Glib::ustring flags_to_string(GFlagsValue * vals, guint flags);

	void param_spec_fraction();
	void param_spec_flags();
	void param_array();
	void param_spec_pointer();
	void param_spec_enum();
public:
	VariousPropertyInspectorView(QTreeWidgetItem* item, GValue value, GParamSpec* param);
	void generate_various_property();
};

#endif /* VARIOUSPROPERTYINSPECTORVIEW_H_ */
