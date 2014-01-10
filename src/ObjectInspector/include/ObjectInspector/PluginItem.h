/*
 * PluginItem.h
 *
 * Plik zawiera definicję klasy PluginItem
 *
 *  Created on: 8 paÅº 2013
 *      Author: Marcin Kolny <marcin.kolny[at]gmail.com>
 */

#ifndef PLUGINITEM_H_
#define PLUGINITEM_H_

#include "../ObjectInspector/ObjectInspectorItem.h"
#include <gstreamermm.h>

/**
 * Klasa reprezentuje pojedynczy element wtyczki
 */
class PluginItem : public ObjectInspectorItem
{
public:
	/**
	 * konstruktor
	 *
	 * @param plugin model wtyczki w bibliotece GStreamer
	 * @param parent obiekt nadrzędny
	 */
	explicit PluginItem(const Glib::RefPtr<Gst::Plugin>& plugin,
			ObjectInspectorItem* parent = 0);

	/**
	 * Zwraca nazwę wtyczki
	 *
	 * @returns nazwa wtyczki
	 */
	virtual std::string get_name() const;

	/**
	 * Zwraca opis wtyczki
	 *
	 * @returns opis wtyczki
	 */
	virtual std::string get_desc() const;

private:
	/** model wtyczki w bibliotece GStreamer */
	Glib::RefPtr<Gst::Plugin> plugin;
};



#endif /* PLUGINITEM_H_ */
