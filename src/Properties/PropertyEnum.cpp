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
  value(value)
{
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
	// TODO
}


