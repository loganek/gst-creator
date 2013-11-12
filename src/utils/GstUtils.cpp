/*
 * GstUtils.cpp
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#include "GstUtils.h"
#include "StringUtils.h"

using namespace Gst;
using Glib::RefPtr;

RefPtr<Gst::Element> GstUtils::find_element(std::string text, const RefPtr<Pipeline>& model)
{
	std::vector<std::string> elements = StringUtils::split(text, ":");
	RefPtr<Bin> current_bin = model;

	for (int i = 0; i < elements.size() - 1; i++)
		current_bin = current_bin.cast_static(model->get_element(elements[i].c_str()));

	return current_bin->get_element(elements.back().c_str());
}

RefPtr<Pad> GstUtils::find_pad(std::string text, const RefPtr<Pipeline>& model)
{
	unsigned pos = text.find_last_of(":");
	RefPtr<Element> element = find_element(text.substr(0, pos), model);
	std::string pad_name = text.substr(pos + 1);

	return element->get_static_pad(pad_name.c_str());
}

bool GstUtils::is_numeric_type(GType type)
{
	return (type == G_TYPE_ULONG) || (type == G_TYPE_LONG)
			|| (type == G_TYPE_UINT) || (type == G_TYPE_INT)
			|| (type == G_TYPE_UINT64) || (type == G_TYPE_INT64)
			|| (type == G_TYPE_FLOAT) || (type == G_TYPE_DOUBLE);
}

std::vector<std::string> GstUtils::get_avaliable_elements_string()
{
	std::vector<std::string> values;

	RefPtr<Registry> registry = Registry::get();
	Glib::ListHandle<RefPtr<Plugin>> plugins = registry->get_plugin_list();

	for (auto plugin : plugins)
	{
		Glib::ListHandle<RefPtr<PluginFeature>> features = registry->get_feature_list(plugin->get_name());

		for (auto feature : features)
		{
			if (!feature)
				continue;

			values.push_back(feature->get_name());
		}
	}

	return values;
}

std::vector<std::string> GstUtils::get_elements_from_bin_string(const RefPtr<Bin>& bin, bool must_bin, const std::string& prefix)
{
	std::vector<std::string> values;

	Iterator<Element> iterator = bin->iterate_elements();

	while (iterator.next())
	{
		if (GST_IS_BIN(iterator->gobj()))
		{
			RefPtr<Bin> internal_bin = internal_bin.cast_static(*iterator);
			std::vector<std::string> internal_values =
					get_elements_from_bin_string(internal_bin, must_bin, iterator->get_name().c_str() + std::string(":"));

			values.insert(values.end(), internal_values.begin(), internal_values.end());
			values.push_back(prefix + iterator->get_name().c_str());
		}
		else if (!must_bin)
			values.push_back(prefix + iterator->get_name().c_str());
	}

	return values;
}

std::vector<std::string> GstUtils::get_properties_string(const RefPtr<Element>& element)
{
	std::vector<std::string> values;
	guint property_count;
	GParamSpec **property_specs = g_object_class_list_properties(
			G_OBJECT_GET_CLASS(element->gobj()), &property_count);

	for (int i = 0; i < property_count; i++)
		values.push_back(property_specs[i]->name);

	return values;
}
