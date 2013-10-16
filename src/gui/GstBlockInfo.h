/*
 * GstBlockInfo.h
 *
 *  Created on: 16 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef GSTBLOCKINFO_H_
#define GSTBLOCKINFO_H_

class GstBlockInfo : public QObject
{
private:
	QString name;
	QPoint location;
	QPixmap pixmap;
	QRect rect;

public:
	explicit GstBlockInfo(QObject* parent = 0);

	QString get_name() const
	{
		return name;
	}

	void set_name(const QString& name)
	{
		this->name = name;
	}

	QString get_location() const
	{
		return location;
	}

	void set_location(const QString& location)
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

#endif /* GSTBLOCKINFO_H_ */
