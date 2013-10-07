/*
 * ElementFactoryItem.cpp
 *
 *  Created on: 7 paź 2013
 *      Author: loganek
 */

#include "ElementFactoryItem.h"
#include <algorithm>

ElementFactoryItem::ElementFactoryItem(const Glib::RefPtr<Gst::ElementFactory>& factory,
		ElementFactoryItem* parent)
: factory(factory),
  parent_item(parent)
{
}

ElementFactoryItem::~ElementFactoryItem()
{
	for (auto item : children)
		delete item;
}

void ElementFactoryItem::append_child(ElementFactoryItem *item)
{
	children.push_back(item);
}

ElementFactoryItem* ElementFactoryItem::child(int row)
{
	return children.at(row);
}

int ElementFactoryItem::child_count() const
{
	return children.size();
}

ElementFactoryItem* ElementFactoryItem::parent()
{
	return parent_item;
}

std::string ElementFactoryItem::get_name() const
{
	return (factory) ?
		factory->get_name().c_str() :
		std::string();
}

std::string ElementFactoryItem::get_long_name() const
{
	return (factory) ?
			factory->get_metadata(GST_ELEMENT_METADATA_LONGNAME).c_str() :
			std::string();
}

int ElementFactoryItem::row() const
{
	if (parent_item)
	{
		return std::find(parent_item->children.begin(), parent_item->children.end(), this) - parent_item->children.begin();
	}

	return 0;
}

const std::string& ElementFactoryItem::get_header(int index)
{
	static std::string header_short_name = "Short Name";
	static std::string header_long_name = "Long Name";
	static std::string header_invalid = "";

	switch (index)
	{
	case 0:
		return header_short_name;
	case 1:
		return header_long_name;
	default:
		return header_invalid;
	}
}
