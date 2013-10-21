/*
 * GstBlock.h
 *
 *  Created on: 14 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef GSTBLOCK_H_
#define GSTBLOCK_H_

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>

enum class PadDirection
{
	INPUT,
	OUTPUT
};

class GstBlock : public QFrame
{
	Q_OBJECT

private:
	void setup_layout();

	QVBoxLayout* left_layout;
	QVBoxLayout* right_layout;

	QLabel* name_label;

public:
	explicit GstBlock(QWidget *parent = 0);
	virtual ~GstBlock();
	void set_name(const QString& name);
	void add_pad(const QString& pad_name, PadDirection dir);
	void remove_pad(const QString& pad_name);
};

#endif /* GSTBLOCK_H_ */
