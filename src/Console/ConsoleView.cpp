/*
 * ConsoleView.cpp
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#include "ConsoleView.h"
#include "utils/EnumUtils.h"
#include "utils/StringUtils.h"

ConsoleView::ConsoleView(CommandListener* listener, QWidget* parent)
: QWidget(parent),
  listener(listener),
  parser(nullptr)
{
	edit = new QLineEdit();
	button = new QPushButton("Execute");

	make_suggestion_widget();

	edit->installEventFilter(this);

	QHBoxLayout* lay = new QHBoxLayout();

	lay->addWidget(edit);
	lay->addWidget(button);

	QObject::connect(button, &QPushButton::clicked, this, &ConsoleView::execute_command);
	QObject::connect(edit, &QLineEdit::returnPressed, this, &ConsoleView::execute_command);
	QObject::connect(edit, &QLineEdit::textChanged, this, &ConsoleView::suggest);
	setLayout(lay);
}

ConsoleView::~ConsoleView()
{
	delete parser;
}

void ConsoleView::make_suggestion_widget()
{
	suggestions = new QTreeWidget;
	suggestions->setWindowFlags(Qt::Popup);
	suggestions->setFocusPolicy(Qt::NoFocus);
	suggestions->setMouseTracking(true);
	suggestions->setColumnCount(1);
	suggestions->setRootIsDecorated(false);
	suggestions->setFrameStyle(QFrame::Box | QFrame::Plain);
	suggestions->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	suggestions->header()->hide();
	suggestions->installEventFilter(this);
}

bool ConsoleView::eventFilter(QObject *obj, QEvent *ev)
{
	if (obj == suggestions && ev->type() == QEvent::MouseButtonPress)
	{
		suggestions->hide();
		edit->setFocus();

		return true;
	}

	else if (ev->type() != QEvent::KeyPress)
		return false;

	QKeyEvent* key_event = static_cast<QKeyEvent*>(ev);
	int key = key_event->key();

	if (obj == suggestions )
	{
		if (key== Qt::Key_Space && key_event->modifiers().testFlag(Qt::ControlModifier))
		{
			edit->setFocus();
			return true;
		}
		switch (key)
		{
		case Qt::Key_Enter:
		case Qt::Key_Return:
			if (suggestions->currentItem())
			{
				command_args.pop_back();
				edit->setText(
						StringUtils::join(command_args, " ").c_str() +
						suggestions->currentItem()->text(0));
			}
		case Qt::Key_Escape:
			edit->setFocus();
			suggestions->hide();
			return true;

		case Qt::Key_Down:
		case Qt::Key_Up:
		case Qt::Key_Home:
		case Qt::Key_End:
		case Qt::Key_PageUp:
		case Qt::Key_PageDown:
			break;
		default:
			edit->setFocus();
			edit->event(ev);
			break;
		}
	}

	else if (obj == edit && key== Qt::Key_Space && key_event->modifiers().testFlag(Qt::ControlModifier))
	{
		make_suggestion(edit->text(), true);
		return true;
	}

	return false;
}

void ConsoleView::execute_command()
{
	try
	{
		std::shared_ptr<Command> cmd(parser->parse(edit->text().toUtf8().constData()));
		cmd->run_command(listener);
		Q_EMIT command_added(cmd);
		//edit->clear();
	}
	catch (const std::runtime_error& err)
	{
		QMessageBox msg_box;
		msg_box.setText(err.what());
		msg_box.exec();
	}
}

void ConsoleView::set_model(const Glib::RefPtr<Gst::Pipeline>& model)
{
	delete parser;
	this->model = model;
	parser = new CommandParser(model);
}

void ConsoleView::suggest(const QString& text)
{
	make_suggestion(text, false);
}

void ConsoleView::make_suggestion(const QString& text, bool autocomplete)
{
	const QPalette &pal = edit->palette();
	QColor color = pal.color(QPalette::Disabled, QPalette::WindowText);

	std::string line = edit->text().toUtf8().constData();
	command_args = StringUtils::split(StringUtils::trim(line), " ");

	if (line.length() > 0 && line[line.length() - 1] == ' ')
		command_args.push_back("");
	std::string cur_text = command_args.back();
	std::string upper_cur_text = StringUtils::to_upper(cur_text);
	std::vector<std::string> vals = get_current_suggestions(command_args);

	for (int i = 0; i < vals.size(); i++)
	{
		if (cur_text.size() > vals[i].size() ||
				(vals[i].substr(0, cur_text.size()) != cur_text &&
						vals[i].substr(0, cur_text.size()) != upper_cur_text))
		{
			vals.erase(vals.begin() + i);
			i--;
		}
	}

	if (vals.size() == 0)
	{
		suggestions->hide();
		return;
	}

	if (vals.size() == 1 && autocomplete)
	{
		command_args.pop_back();
		std::string out = StringUtils::join(command_args, " ") + vals[0].c_str();
		edit->setText(out.c_str());
		return;
	}

	suggestions->setUpdatesEnabled(false);
	suggestions->clear();

	for (int i = 0; i < vals.size(); ++i)
	{
		QTreeWidgetItem * item;
		item = new QTreeWidgetItem(suggestions);
		item->setText(0, vals[i].c_str());
	}

	suggestions->setCurrentItem(suggestions->topLevelItem(0));
	suggestions->resizeColumnToContents(0);
	suggestions->adjustSize();
	suggestions->setUpdatesEnabled(true);

	int h = suggestions->sizeHintForRow(0) * std::min((size_t)7, vals.size()) + 3;
	suggestions->resize(suggestions->width(), h);

	suggestions->move(edit->mapToGlobal(QPoint(0, edit->height())));
	suggestions->setFocus();
	suggestions->show();
}


std::vector<std::string> ConsoleView::get_current_suggestions(std::vector<std::string> command_args)
{
	if (command_args.size() < 2)
		return EnumUtils<CommandType>::get_string_values();

	try
	{
		CommandType type = EnumUtils<CommandType>::string_to_enum(command_args[0]);
		command_args.erase(command_args.begin());

		switch (type)
		{
		case CommandType::ADD:
			return AddCommand::get_suggestions(command_args, model);
		case CommandType::STATE:
			return StateCommand::get_suggestions(command_args, model);
		case CommandType::CONNECT:
			return ConnectCommand::get_suggestions(command_args, model);
		case CommandType::PROPERTY:
			return PropertyCommand::get_suggestions(command_args, model);
		case CommandType::REMOVE:
			return RemoveCommand::get_suggestions(command_args, model);
		case CommandType::DISCONNECT:
			return DisconnectCommand::get_suggestions(command_args, model);
		}
	}
	catch (...)
	{
		return std::vector<std::string>();
	}

	return std::vector<std::string>();
}
