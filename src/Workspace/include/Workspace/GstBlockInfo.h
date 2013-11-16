/*
 * GstBlockInfo.h
 *
 *  Created on: 16 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef GSTBLOCKINFO_H_
#define GSTBLOCKINFO_H_

#include "GstBlock.h"
#include <QObject>
#include <QtCore>
#include <QtGui>

class GstBlockInfo : public QObject
{
	Q_OBJECT
private:
	GstBlock* block;
	QPoint location;
	QRect rect;

public:
	explicit GstBlockInfo(GstBlock* block, const QPoint& location,
			const QRect& rect, QObject* parent = 0)
	: QObject(parent),
	  block(block),
	  location(location),
	  rect(rect)
	{}

	virtual ~GstBlockInfo(){}

	GstBlock* get_block()
	{
		return block;
	}

	QString get_name() const
	{
		return block->get_model()->get_name().c_str();
	}

	QPoint get_location() const
	{
		return location;
	}

	void set_location(const QPoint& location)
	{
		this->location = location;
	}

	QPixmap get_pixmap() const
	{
		return block->grab();
	}

	QRect get_rect() const
	{
		return rect;
	}

	void set_rect(const QRect& rect)
	{
		this->rect = rect;
	}
};

#endif /* GSTBLOCKINFO_H_ */
