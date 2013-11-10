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
private:
	std::string value;

protected:
	virtual void build_widget();
public:
	PropertyEnum(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element,
			const std::string& value);
	QWidget* get_widget();

	void set_value();
};

#endif /* PROPERTYENUM_H_ */
