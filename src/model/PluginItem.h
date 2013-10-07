/*
 * PluginItem.h
 *
 *  Created on: 8 paź 2013
 *      Author: Marcin Kolny <marcin.kolny[at]gmail.com>
 */

#ifndef PLUGINITEM_H_
#define PLUGINITEM_H_

#include "FactoryItem.h"
#include <gstreamermm.h>

class PluginItem : public FactoryItem
{
public:
	explicit PluginItem(const Glib::RefPtr<Gst::Plugin>& plugin,
			FactoryItem* parent = 0);

	virtual std::string get_name() const;
	virtual std::string get_desc() const;

private:
	Glib::RefPtr<Gst::Plugin> plugin;
};



#endif /* PLUGINITEM_H_ */
