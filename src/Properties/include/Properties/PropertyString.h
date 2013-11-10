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
private:
	std::string value;

	QLineEdit* edit;

protected:
	virtual void build_widget();
	virtual void init();
public:
	PropertyString(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element,
			const std::string& value);
	QWidget* get_widget();

	void set_value();
};

#endif /* PROPERTYSTRING_H_ */
