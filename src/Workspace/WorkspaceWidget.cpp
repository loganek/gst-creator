/*
 * WorkspaceWidget.cpp
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#include "WorkspaceWidget.h"
#include "GstBlock.h"
#include "common.h"
#include "Commands.h"
#include <QtGui>
#include <QFrame>

WorkspaceWidget::WorkspaceWidget(const Glib::RefPtr<Gst::Pipeline>& model, QWidget* parent)
: QWidget(parent),
  model(model),
  current_info(nullptr)
{
	setAcceptDrops(true);
}

WorkspaceWidget::~WorkspaceWidget()
{
}

bool WorkspaceWidget::check_mime_data(const QMimeData* mime_data) const
{
	return mime_data->hasFormat(DRAG_DROP_FORMAT);
}

void WorkspaceWidget::set_style_sheet(const char* style_sheet)
{
	QFrame* frame = static_cast<QFrame*> (this->parent());
	frame->setStyleSheet(style_sheet);
}

void WorkspaceWidget::dragEnterEvent(QDragEnterEvent* event)
{
	if (!check_mime_data(event->mimeData()))
	{
		event->ignore();
		return;
	}

	set_style_sheet(active_style_sheet);
	event->accept();
}

void WorkspaceWidget::dragMoveEvent(QDragMoveEvent* event)
{
	if (!check_mime_data(event->mimeData()))
	{
		event->ignore();
		return;
	}

	event->accept();

	repaint();
}

void WorkspaceWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
	set_style_sheet(passive_style_sheet);
	repaint();
}

QRect generate_rectangle(const QPoint& location)
{
	return QRect(location.x(), location.y(), GstBlock::get_width(), GstBlock::get_height());
}
QString WorkspaceWidget::get_new_name(const QString& name)
{
	bool ok;
	QString new_name = QInputDialog::getText(this, tr("QInputDialog::getText()"),
			tr("Element name:"), QLineEdit::Normal, name, &ok);
	if (ok && !new_name.isEmpty())
		return new_name;
	else
		return QString();
}
void WorkspaceWidget::dropEvent(QDropEvent* event)
{
	if (!check_mime_data(event->mimeData()))
	{
		event->ignore();
		return;
	}

	set_style_sheet(passive_style_sheet);

	bool present = false;
	QByteArray item_data = event->mimeData()->data(DRAG_DROP_FORMAT);
	QDataStream data_stream(&item_data, QIODevice::ReadOnly);
	QPoint location;
	int val;

	data_stream >> location >> val;
	GstBlock* element = reinterpret_cast<GstBlock*>(val); // fixme ugly code

	QRect rectangle = generate_rectangle(event->pos() - location);

	if (blocks.size() > 0)
	{
		for (auto block_info : blocks)
		{
			if (block_info->get_block()->get_model() == element->get_model())
			{
				present = true;
				current_info = block_info;
				current_info->set_rect(rectangle);
				current_info->set_location(location);
				Q_EMIT current_element_changed(current_info->get_block()->get_model());
				break;
			}
		}
	}

	if (blocks.size() == 0 || (blocks.size() > 0 && !present))
	{
		QString new_name = get_new_name(element->get_model()->get_name().c_str());
		element->get_model()->set_name(new_name.toUtf8().constData());
		GstBlockInfo* info = new GstBlockInfo(element, location, rectangle);
		blocks.push_back(info);
		AddCommand cmd(ObjectType::ELEMENT, model, element->get_model());
		cmd.run_command();
	}

	repaint();

	if (event->source() == this)
	{
		event->setDropAction(Qt::MoveAction);
		event->accept();
	}
	else
		event->acceptProposedAction();
}


void WorkspaceWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter;
	painter.begin(this);

	for (auto info : blocks)
		painter.drawPixmap(info->get_rect(), info->get_pixmap());
}

void WorkspaceWidget::mousePressEvent(QMouseEvent* event)
{
	QRect rectangle = generate_rectangle(event->pos());

	current_info = nullptr;

	for (GstBlockInfo* info : blocks)
	{
		if (info->get_rect().contains(rectangle.x(),rectangle.y()))
			current_info = info;
	}

	this->setFocus();

	if (current_info == nullptr)
		return;

	QPoint position(current_info->get_rect().x(), current_info->get_rect().y());
	QPoint location = event->pos() - position;

	QByteArray item_data;
	QDataStream data_stream(&item_data, QIODevice::WriteOnly);

	data_stream << location << reinterpret_cast<int>(current_info->get_block());

	QMimeData* mime_data = new QMimeData;
	mime_data->setData(DRAG_DROP_FORMAT, item_data);

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mime_data);
	drag->setHotSpot(location);
	drag->setPixmap(current_info->get_pixmap());
	drag->exec(Qt::MoveAction | Qt::CopyAction);

	repaint();

	Q_EMIT current_element_changed(current_info->get_block()->get_model());
}

void WorkspaceWidget::model_changed(std::shared_ptr<Command> cmd)
{
	switch (cmd->get_type())
	{
	case CommandType::ADD:
	{
		Glib::RefPtr<Gst::Object> ob = ob.cast_static(std::static_pointer_cast<AddCommand>(cmd)->get_object());
		GstBlockInfo* info = new GstBlockInfo(new GstBlock(Glib::RefPtr<Gst::Element>::cast_static(ob)),
				QPoint(10, 10), QRect(0, 0, GstBlock::get_width(), GstBlock::get_height()));
		blocks.push_back(info);
		repaint();
	}
	break;
	case CommandType::PROPERTY:
		break;
	}
}
