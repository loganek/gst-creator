/*
 * PropertyNumeric.h
 *
 *  Created on: 9 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef PROPERTYNUMERIC_H_
#define PROPERTYNUMERIC_H_

#include "Property.h"
#include <string>
#include <sstream>

template<typename T>
class PropertyNumeric : public Property
{
private:
	T value;

protected:
	virtual void build_widget();
public:
	PropertyNumeric(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element,
			T value);
	QWidget* get_widget();
	void set_value();
	void set_value(const std::string& value);

	typedef T command_type;
};

template<typename T>
PropertyNumeric<T>::PropertyNumeric(GParamSpec* param_spec,
		const Glib::RefPtr<Gst::Element>& element, T value)
		: Property(param_spec, element),
		  value(value)
		{}

template<typename T>
void PropertyNumeric<T>::set_value()
{
	element->property(param_spec->name, value);
}

template<typename T>
void PropertyNumeric<T>::build_widget()
{
	widget = new QFrame();
	widget->setLayout(new QHBoxLayout);
	widget->layout()->addWidget(new QLabel(param_spec->name));
	widget->layout()->addWidget(new QLineEdit());
}

#endif /* PROPERTYNUMERIC_H_ */
