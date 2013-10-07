/*
 * PluginItem.cpp
 *
 *  Created on: 8 paź 2013
 *      Author: Marcin Kolny <marcin.kolny[at]gmail.com>
 */

#include "PluginItem.h"

PluginItem::PluginItem(const Glib::RefPtr<Gst::Plugin>& plugin,
		FactoryItem* parent)
: FactoryItem(parent),
  plugin(plugin)
{}

std::string PluginItem::get_name() const
{
	return plugin->get_name();
}

std::string PluginItem::get_desc() const
{
	return plugin->get_description();
}


