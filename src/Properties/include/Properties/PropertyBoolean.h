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
public:
	PropertyBoolean(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element,
			bool value);

	void set_value();
};

#endif /* PROPERTYBOOLEAN_H_ */