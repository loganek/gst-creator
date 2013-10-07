/*
 * FactoryItem.cpp
 *
 *  Created on: 7 paź 2013
 *      Author: Marcin Kolny <marcin.kolny[at]gmail.com>
 */

#include "FactoryItem.h"
#include <algorithm>

FactoryItem::FactoryItem(FactoryItem* parent)
: parent_item(parent)
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

int FactoryItem::row() const
{
	if (parent_item)
	{
		return std::find(parent_item->children.begin(), parent_item->children.end(), this) - parent_item->children.begin();
	}

	return 0;
}


std::string FactoryItem::get_name() const
{
	return get_header(0);
}

std::string FactoryItem::get_desc() const
{
	return get_header(1);
}

const std::string& FactoryItem::get_header(int index)
{
	static std::string name = "Name";
	static std::string desc = "Description";
	static std::string header_invalid = "";

	switch (index)
	{
	case 0:
		return name;
	case 1:
		return desc;
	default:
		return header_invalid;
	}
}
