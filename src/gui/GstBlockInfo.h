/*
 * GstBlockInfo.h
 *
 *  Created on: 16 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef GSTBLOCKINFO_H_
#define GSTBLOCKINFO_H_

#include <QObject>
#include <QtCore>
#include <QtGui>
#include <iostream>

class GstBlockInfo : public QObject
{
	Q_OBJECT
private:
	QString name;
	QPoint location;
	QPixmap pixmap;
	QRect rect;

public:
	explicit GstBlockInfo(const QPixmap& pixmap, const QPoint& location,
			const QString& name, const QRect& rect, QObject* parent = 0)
	: QObject(parent),
	  pixmap(pixmap),
	  location(location),
	  name(name),
	  rect(rect)
	{}

	virtual ~GstBlockInfo(){}

	static std::vector<GstBlockInfo*>& get_blocks()
	{
		static std::vector<GstBlockInfo*> blocks;

		return blocks;
	}

	QString get_name() const
	{
		return name;
	}

	void set_name(const QString& name)
	{
		this->name = name;
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
		return pixmap;
	}

	void set_pixmap(const QPixmap& pixmap)
	{
		this->pixmap = pixmap;
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

//std::vector<GstBlockInfo*> GstBlockInfo::blocks;

#endif /* GSTBLOCKINFO_H_ */
