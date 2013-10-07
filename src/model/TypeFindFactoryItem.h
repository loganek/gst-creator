/*
 * TypeFindFactoryItem.h
 *
 *  Created on: 7 paź 2013
 *      Author: Marcin Kolny <marcin.kolny[at]gmail.com>
 */

#ifndef TYPEFINDFACTORYITEM_H_
#define TYPEFINDFACTORYITEM_H_

#include "FactoryItem.h"
#include <gstreamermm.h>

class TypeFindFactoryItem : public FactoryItem
{
public:
	explicit TypeFindFactoryItem(const Glib::RefPtr<Gst::TypeFindFactory>& factory,
			FactoryItem* parent = 0);

	virtual std::string get_name() const;
	virtual std::string get_desc() const;

private:
	Glib::RefPtr<Gst::TypeFindFactory> factory;
};


#endif /* TYPEFINDFACTORYITEM_H_ */
