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


int ElementFactoryItem::row() const
{
	if (parent_item)
	{
		return std::find(parent_item->children.begin(), parent_item->children.end(), this) - parent_item->children.begin();
	}

	return 0;
}

