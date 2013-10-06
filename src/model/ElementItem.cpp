/*
 * ElementItem.cpp
 *
 *  Created on: 7 paź 2013
 *      Author: loganek
 */

#include "ElementItem.h"
#include <algorithm>

ElementItem::ElementItem(const Glib::RefPtr<Gst::Element>& element,
		ElementItem* parent)
: element(element),
  parent_item(parent)
{
}

ElementItem::~ElementItem()
{
	for (auto item : children)
		delete item;
}

void ElementItem::append_child(ElementItem *item)
{
	children.push_back(item);
}

ElementItem* ElementItem::child(int row)
{
	return children.at(row);
}

int ElementItem::child_count() const
{
	return children.size();
}

ElementItem* ElementItem::parent()
{
	return parent_item;
}


int ElementItem::row() const
{
	if (parent_item)
	{
		return std::find(parent_item->children.begin(), parent_item->children.end(), this) - parent_item->children.begin();
	}

	return 0;
}

