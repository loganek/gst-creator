/*
 * ObjectInspectorItem.cpp
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#include "ObjectInspectorItem.h"
#include <algorithm>

ObjectInspectorItem::ObjectInspectorItem(const std::string& name, FactoryType factory_type, ObjectInspectorItem* parent)
: parent_item(parent),
  factory_name(name),
  factory_type(factory_type)
{
}

ObjectInspectorItem::~ObjectInspectorItem()
{
	for (auto item : children)
		delete item;
}

void ObjectInspectorItem::append_child(ObjectInspectorItem *item)
{
	children.push_back(item);
}

ObjectInspectorItem* ObjectInspectorItem::child(int row)
{
	return children.at(row);
}

int ObjectInspectorItem::child_count() const
{
	return children.size();
}

ObjectInspectorItem* ObjectInspectorItem::parent()
{
	return parent_item;
}

int ObjectInspectorItem::row() const
{
	if (parent_item)
	{
		return std::find(parent_item->children.begin(),
				parent_item->children.end(), this) - parent_item->children.begin();
	}

	return -1;
}


std::string ObjectInspectorItem::get_name() const
{
	static std::string header_name = "Factory Name";

	return factory_name.empty() ? header_name : factory_name;
}


