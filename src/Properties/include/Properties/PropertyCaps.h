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

protected:
	virtual void build_widget();
public:
	PropertyCaps(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element,
			const Glib::RefPtr<Gst::Caps>& value);
	QWidget* get_widget();

	void set_value();
};

#endif /* PROPERTYCAPS_H_ */
