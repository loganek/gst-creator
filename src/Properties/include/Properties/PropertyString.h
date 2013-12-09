/*
 * PropertyString.h
 *
 *  Created on: 9 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef PROPERTYSTRING_H_
#define PROPERTYSTRING_H_

#include "Property.h"

class PropertyString : public Property
{
	Q_OBJECT
private:
	std::string value;

	QLineEdit* edit;

	public Q_SLOTS:
	void update_value(const QString& new_value);

	protected:
	virtual void build_widget();
	virtual void init();
	void read_var();

public:
	PropertyString(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element,
			const std::string& value);
	PropertyString(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element);

	void set_value();
	std::string get_str_value() const;
	bool is_default_value() const;
	std::string get_type_name() const;
};

#endif /* PROPERTYSTRING_H_ */
