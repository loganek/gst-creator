/*
 * ElementItem.h
 *
 *  Created on: 7 paź 2013
 *      Author: loganek
 */

#ifndef ELEMENTITEM_H_
#define ELEMENTITEM_H_

#include <gstreamermm.h>
#include <vector>

class ElementItem
{
public:
	explicit ElementItem(const Glib::RefPtr<Gst::Element>& element,
			ElementItem *parent = 0);
	~ElementItem();

	void append_child(ElementItem* child);

	ElementItem *child(int row);
	int child_count() const;
	int row() const;
	ElementItem *parent();

private:
	std::vector<ElementItem*> children;
	Glib::RefPtr<Gst::Element> element;
	ElementItem* parent_item;
};


#endif /* ELEMENTITEM_H_ */
