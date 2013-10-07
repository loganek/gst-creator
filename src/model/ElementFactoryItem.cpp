/*
 * ElementFactoryItem.cpp
 *
 *  Created on: 7 paź 2013
 *      Author: Marcin Kolny <marcin.kolny[at]gmail.com>
 */

#include "ElementFactoryItem.h"

ElementFactoryItem::ElementFactoryItem(const Glib::RefPtr<Gst::ElementFactory>& factory,
		FactoryItem* parent)
: FactoryItem(parent),
  factory(factory)
{}

std::string ElementFactoryItem::get_name() const
{
	return factory->get_name();
}

std::string ElementFactoryItem::get_desc() const
{
	return factory->get_metadata(GST_ELEMENT_METADATA_LONGNAME);
}

