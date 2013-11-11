/*
 * PropertyCommand.cpp
 *
 *  Created on: 9 lis 2013
 *      Author: Marcin Kolny
 */

#include "PropertyCommand.h"
#include "utils/EnumUtils.h"
#include "utils/GstUtils.h"
#include "Properties/Property.h"

using namespace Gst;
using Glib::RefPtr;

PropertyCommand::PropertyCommand(const RefPtr<Element>& element,
		const std::string& property_name, const std::string& property_value)
: element(element)
{
	run_window = property_name.empty();

	if (run_window)
		property = nullptr;
	else
	{
		GParamSpec* param = g_object_class_find_property(
				G_OBJECT_GET_CLASS(element->gobj()), property_name.c_str());

		property = Property::build_property(param, element, property_value);
	}
}

PropertyCommand::~PropertyCommand()
{
	delete property;
}

void PropertyCommand::run_command()
{
	if (run_window)
	{
		QDialog* dlg = new QDialog();
		dlg->setLayout(new QVBoxLayout());
		dlg->layout()->addWidget(
				Property::build_property_window(element));

		class TmpClass : public QObject
		{
		public:
			bool eventFilter(QObject *obj, QEvent *evt)
			{
				if(evt->type() == QEvent::KeyPress) {
					QKeyEvent *keyEvent = static_cast<QKeyEvent*>(evt);
					if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return )
						return true;
				}
				return false;
			}
		}tmp_obj;
		// TODO build custom Dialog class with disabled enters

		dlg->installEventFilter(&tmp_obj);
		dlg->exec();
	}
	else if (property != nullptr)
		property->set_value();
	else
		throw std::runtime_error("Cannot set property. Property unavailable.");
}

PropertyCommand* PropertyCommand::from_args(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model)
{
	RefPtr<Element> element = GstUtils::find_element(args[0], model);

	if (!element)
		throw std::runtime_error("cannot find element " + args[0]);

	if (args.size() != 3 && args.size() != 1)
		syntax_error("invalid arguments count. Expected 1 or 3, but " + std::to_string(args.size()) + " found.");

	if (args.size() == 1)
		return new PropertyCommand(element, "", "");

	return new PropertyCommand(element, args[1], args[2]);
}