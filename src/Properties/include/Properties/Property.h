/*
 * Property.h
 *
 *  Created on: 9 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef PROPERTY_H_
#define PROPERTY_H_

#include <gstreamermm.h>
#include <QtWidgets>
#include <memory>

class Property : public QObject
{
private:
	bool was_built;
protected:
	GParamSpec* param_spec;
	Glib::RefPtr<Gst::Element> element;
	QWidget* widget;

	virtual void build_widget() = 0;
	virtual void init() = 0;
	static Property* build_numeric_property(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element, GType type,
			const std::string& value);
public:
	Property(GParamSpec* param_spec, const Glib::RefPtr<Gst::Element>& element);
	virtual ~Property();
	QWidget* get_widget();
	virtual void set_value() = 0;

	static Property* build_property(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element,
			const std::string& value);

	static QWidget* build_property_window(
			const Glib::RefPtr<Gst::Element>& element);
};

#endif /* PROPERTY_H_ */
