/*
 * PropertyCaps.h
 *
 *  Created on: 10 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef PROPERTYCAPS_H_
#define PROPERTYCAPS_H_

#include "Property.h"

class PropertyCaps : public Property
{
private:
	Glib::RefPtr<Gst::Caps> value;
	Glib::ustring tmp_value;
	QLineEdit* caps_edit;

	void read_var();

protected:
	virtual void build_widget();
	virtual void init();
public:
	PropertyCaps(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element,
			const Glib::RefPtr<Gst::Caps>& value);
	PropertyCaps(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element);

	bool is_default_value() const;
	void set_value();
	std::string get_str_value() const;
	std::string get_type_name() const;
};

#endif /* PROPERTYCAPS_H_ */
