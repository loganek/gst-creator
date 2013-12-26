/*
 * PropertyEnum.cpp
 *
 *  Created on: 10 lis 2013
 *      Author: Marcin Kolny
 */

#include "PropertyEnum.h"
#include "utils/StringUtils.h"

PropertyEnum::PropertyEnum(GParamSpec* param_spec,
		const Glib::RefPtr<Gst::Element>& element,
		const std::string& value)
: Property(param_spec, element),
  value(value),
  combobox(nullptr)
{
}

PropertyEnum::PropertyEnum(GParamSpec* param_spec,
		const Glib::RefPtr<Gst::Element>& element)
: Property(param_spec, element)
{
	read_var();
}

void PropertyEnum::set_value()
{
	int pos;

	if (StringUtils::is_number(value))
		pos = StringUtils::str_to_numeric<int>(value);
	else
	{
		GEnumValue *values;
		guint j = 0;
		values = G_ENUM_CLASS (g_type_class_ref(param_spec->value_type))->values;

		while (values[j].value_name)
		{
			if (value == values[j].value_nick)
			{
				pos = values[j].value;
				break;
			}
			j++;
		}
	}
	element->property(param_spec->name, pos);
}

void PropertyEnum::build_widget()
{
	combobox = new QComboBox();
	widget->layout()->addWidget(combobox);

	GEnumValue *values;
	guint j = 0;
	values = G_ENUM_CLASS (g_type_class_ref(param_spec->value_type))->values;

	while (values[j].value_name)
	{
		combobox->addItems({values[j].value_nick});
		j++;
	}
	connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(update_position(int)));
}

void PropertyEnum::init()
{
	read_var();

	if (StringUtils::is_number(value))
		combobox->setCurrentIndex(StringUtils::str_to_numeric<int>(value));
}

void PropertyEnum::update_position(int pos)
{
	value = std::to_string(pos);
	set_value();
	init();
}

std::string PropertyEnum::get_str_value() const
{
	return value;
}

void PropertyEnum::read_var()
{
	gint enum_value;
	GValue g_value = { 0, };
	g_value_init (&g_value, param_spec->value_type);
	g_object_get_property(G_OBJECT (element->gobj()), param_spec->name, &g_value);
	enum_value = g_value_get_enum (&g_value);
	value = std::to_string(enum_value);

}

bool PropertyEnum::is_default_value() const
{
	const GValue* v = g_param_spec_get_default_value(param_spec);
	gint i = g_value_get_enum(v);

	return std::to_string(i) == value;
}

std::string PropertyEnum::get_type_name() const
{
	return "gint";
}
