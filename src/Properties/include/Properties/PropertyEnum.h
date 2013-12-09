/*
 * PropertyEnum.h
 *
 *  Created on: 10 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef PROPERTYENUM_H_
#define PROPERTYENUM_H_

#include "Property.h"

class PropertyEnum : public Property
{
	Q_OBJECT
private:
	std::string value;

	QComboBox* combobox;
protected:
	virtual void build_widget();
	virtual void init();
	void read_var();

	private Q_SLOTS:
	void update_position(int pos);
	public:
	PropertyEnum(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element,
			const std::string& value);
	PropertyEnum(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element);

	void set_value();
	std::string get_str_value() const;
	bool is_default_value() const;
	std::string get_type_name() const;
};

#endif /* PROPERTYENUM_H_ */
