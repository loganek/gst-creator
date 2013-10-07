/*
 * FactoryItem.h
 *
 *  Created on: 7 paź 2013
 *      Author: Marcin Kolny <marcin.kolny[at]gmail.com>
 */

#ifndef FactoryItem_H_
#define FactoryItem_H_

#include <gstreamermm.h>
#include <vector>

class FactoryItem
{
public:
	explicit FactoryItem(const Glib::RefPtr<Gst::ElementFactory>& factory,
			FactoryItem *parent = 0);
	~FactoryItem();

	void append_child(FactoryItem* child);

	FactoryItem *child(int row);
	int child_count() const;
	int row() const;
	FactoryItem *parent();

	std::string get_name() const;
	std::string get_long_name() const;

	static const std::string& get_header(int index);
private:
	std::vector<FactoryItem*> children;
	Glib::RefPtr<Gst::ElementFactory> factory;
	FactoryItem* parent_item;
};


#endif /* FactoryItem_H_ */
