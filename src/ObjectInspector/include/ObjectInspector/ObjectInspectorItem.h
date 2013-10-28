/*
 * ObjectInspectorItem.h
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef OBJECTINSPECTORITEM_H_
#define OBJECTINSPECTORITEM_H_

#include <string>
#include <vector>

enum class FactoryType
{
	ELEMENT_FACTORY,
	TYPEFIND_FACTORY,
	UNKNOW_FACTORY
};

class ObjectInspectorItem
{
private:
	std::vector<ObjectInspectorItem*> children;
	ObjectInspectorItem* parent_item;
	FactoryType factory_type;
	std::string factory_name;

public:
	explicit ObjectInspectorItem(const std::string& name,
			FactoryType type = FactoryType::UNKNOW_FACTORY, ObjectInspectorItem *parent = 0);
	virtual ~ObjectInspectorItem();

	void append_child(ObjectInspectorItem* child);

	ObjectInspectorItem *child(int row);
	int child_count() const;
	int row() const;
	ObjectInspectorItem *parent();

	virtual std::string get_name() const;

	void set_type(FactoryType type)
	{
		factory_type = type;
	}
};


#endif /* OBJECTINSPECTORITEM_H_ */
