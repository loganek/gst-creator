/*
 * PropertyBoolean.h
 *
 *  Created on: 9 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef PROPERTYBOOLEAN_H_
#define PROPERTYBOOLEAN_H_

#include "Property.h"

class PropertyBoolean : public Property
{
private:
	bool value;

	QCheckBox* checkbox;
protected:
	virtual void build_widget();
	virtual void init();
	void read_var();
public:
	PropertyBoolean(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element,
			bool value);
	PropertyBoolean(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element);

	std::string get_str_value() const;
	void set_value();
	bool is_default_value() const;
	std::string get_type_name() const;
};

#endif /* PROPERTYBOOLEAN_H_ */
