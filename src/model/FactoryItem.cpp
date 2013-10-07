/*
 * FactoryItem.cpp
 *
 *  Created on: 7 paź 2013
 *      Author: Marcin Kolny <marcin.kolny[at]gmail.com>
 */

#include "FactoryItem.h"
#include <algorithm>

FactoryItem::FactoryItem(const Glib::RefPtr<Gst::ElementFactory>& factory,
		FactoryItem* parent)
: factory(factory),
  parent_item(parent)
{
}

FactoryItem::~FactoryItem()
{
	for (auto item : children)
		delete item;
}

void FactoryItem::append_child(FactoryItem *item)
{
	children.push_back(item);
}

FactoryItem* FactoryItem::child(int row)
{
	return children.at(row);
}

int FactoryItem::child_count() const
{
	return children.size();
}

FactoryItem* FactoryItem::parent()
{
	return parent_item;
}

std::string FactoryItem::get_name() const
{
	return (factory) ?
		factory->get_name().c_str() :
		std::string();
}

std::string FactoryItem::get_long_name() const
{
	return (factory) ?
			factory->get_metadata(GST_ELEMENT_METADATA_LONGNAME).c_str() :
			std::string();
}

int FactoryItem::row() const
{
	if (parent_item)
	{
		return std::find(parent_item->children.begin(), parent_item->children.end(), this) - parent_item->children.begin();
	}

	return 0;
}

const std::string& FactoryItem::get_header(int index)
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
