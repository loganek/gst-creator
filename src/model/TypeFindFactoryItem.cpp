/*
 * TypeFindFactoryItem.cpp
 *
 *  Created on: 7 paź 2013
 *      Author: Marcin Kolny <marcin.kolny[at]gmail.com>
 */

#include "TypeFindFactoryItem.h"

TypeFindFactoryItem::TypeFindFactoryItem(const Glib::RefPtr<Gst::TypeFindFactory>& factory,
		FactoryItem* parent)
: FactoryItem(parent),
  factory(factory)
{}

std::string TypeFindFactoryItem::get_name() const
{
	return factory->get_name();
}

std::string TypeFindFactoryItem::get_desc() const
{
	std::vector<Glib::ustring> extensions = factory->get_extensions();

	if (!extensions.empty())
	{
		int i = 0;
		std::string ext_str;

		for (auto extension : extensions)
		{
			if (i++ > 0)
				ext_str += ", ";

			ext_str += extension;
		}

		return ext_str;
	}
	else
		return std::string("no extensions");
}

