/*
 * ElementFactoryItem.h
 *
 *  Created on: 7 paź 2013
 *      Author: Marcin Kolny <marcin.kolny[at]gmail.com>
 */

#ifndef ELEMENTFACTORYITEM_H_
#define ELEMENTFACTORYITEM_H_

#include "FactoryItem.h"
#include <gstreamermm.h>

class ElementFactoryItem : public FactoryItem
{
public:
	explicit ElementFactoryItem(const Glib::RefPtr<Gst::ElementFactory>& factory,
			FactoryItem* parent = 0);

	virtual std::string get_name() const;
	virtual std::string get_desc() const;

private:
	Glib::RefPtr<Gst::ElementFactory> factory;
};


#endif /* ELEMENTFACTORYITEM_H_ */
