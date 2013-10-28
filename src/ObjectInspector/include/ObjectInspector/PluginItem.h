/*
 * PluginItem.h
 *
 *  Created on: 8 paź 2013
 *      Author: Marcin Kolny <marcin.kolny[at]gmail.com>
 */

#ifndef PLUGINITEM_H_
#define PLUGINITEM_H_

#include "../ObjectInspector/ObjectInspectorItem.h"
#include <gstreamermm.h>

class PluginItem : public ObjectInspectorItem
{
public:
	explicit PluginItem(const Glib::RefPtr<Gst::Plugin>& plugin,
			ObjectInspectorItem* parent = 0);

	virtual std::string get_name() const;
	virtual std::string get_desc() const;

private:
	Glib::RefPtr<Gst::Plugin> plugin;
};



#endif /* PLUGINITEM_H_ */
