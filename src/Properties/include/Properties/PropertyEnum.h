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

private Q_SLOTS:
	void update_position(int pos);
public:
	PropertyEnum(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element,
			const std::string& value);

	void set_value();
};

#endif /* PROPERTYENUM_H_ */
