/*
 * ElementFactoryItem.h
 *
 *  Created on: 7 paź 2013
 *      Author: loganek
 */

#ifndef ELEMENTFACTORYITEM_H_
#define ELEMENTFACTORYITEM_H_

#include <gstreamermm.h>
#include <vector>

class ElementFactoryItem
{
public:
	explicit ElementFactoryItem(const Glib::RefPtr<Gst::ElementFactory>& factory,
			ElementFactoryItem *parent = 0);
	~ElementFactoryItem();

	void append_child(ElementFactoryItem* child);

	ElementFactoryItem *child(int row);
	int child_count() const;
	int row() const;
	ElementFactoryItem *parent();

	std::string get_name() const;
	std::string get_long_name() const;

	static const std::string& get_header(int index);
private:
	std::vector<ElementFactoryItem*> children;
	Glib::RefPtr<Gst::ElementFactory> factory;
	ElementFactoryItem* parent_item;
};


#endif /* ELEMENTFACTORYITEM_H_ */
