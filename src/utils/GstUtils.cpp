/*
 * GstUtils.cpp
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#include "GstUtils.h"
#include "StringUtils.h"
#include <vector>

using namespace Gst;
using Glib::RefPtr;

RefPtr<Gst::Element> GstUtils::find_element(std::string text, const RefPtr<Pipeline>& model)
{
	std::vector<std::string> elements = StringUtils::split(text, ":");
	RefPtr<Bin> current_bin = model;

	for (size_t i = 0; i < elements.size() - 1; i++)
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

RefPtr<PadTemplate> GstUtils::find_pad_template(std::string text, const RefPtr<Pipeline>& model)
{
	unsigned pos = text.find_last_of(":");
	RefPtr<Element> element = find_element(text.substr(0, pos), model);
	std::string pad_template_name = text.substr(pos + 1);

	return element->get_pad_template(pad_template_name.c_str());
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

std::vector<std::string> GstUtils::get_pads_from_element_string(const RefPtr<Element>& element)
{
	std::vector<std::string> values;

	if (!element)
		return values;

	typedef Iterator<Pad> PadIter;
	PadIter iter = element->iterate_pads();

	while(iter.next())
	{
		values.push_back(iter->get_name().c_str());
	}

	return values;
}

std::vector<std::string> GstUtils::get_pads_from_bin_string(const RefPtr<Bin>& bin, const std::string& prefix)
{
	std::vector<std::string> values;

	Iterator<Element> iterator = bin->iterate_elements();

	while (iterator.next())
	{
		if (GST_IS_BIN(iterator->gobj()))
		{
			RefPtr<Bin> internal_bin = internal_bin.cast_static(*iterator);
			std::vector<std::string> internal_values =
					get_pads_from_bin_string(internal_bin, iterator->get_name().c_str() + std::string(":"));

			for (auto& p : internal_values) p = prefix + p;
			values.insert(values.end(), internal_values.begin(), internal_values.end());

			std::vector<std::string> pads = get_pads_from_element_string(*iterator);
			for (auto& p : pads) p = prefix +  iterator->get_name().c_str() + std::string(":") + p;
			values.insert(values.end(), pads.begin(), pads.end());
		}

		std::vector<std::string> pads = get_pads_from_element_string(*iterator);
		for (auto& p : pads) p = prefix +  iterator->get_name().c_str() + std::string(":") + p;
		values.insert(values.end(), pads.begin(), pads.end());
	}

	return values;
}

std::vector<std::string> GstUtils::get_properties_string(const RefPtr<Element>& element)
{
	std::vector<std::string> values;
	guint property_count;
	GParamSpec **property_specs = g_object_class_list_properties(
			G_OBJECT_GET_CLASS(element->gobj()), &property_count);

	for (guint i = 0; i < property_count; i++)
		values.push_back(property_specs[i]->name);

	return values;
}

std::vector<std::string> GstUtils::get_avaliable_pad_templates_string(const RefPtr<Element>& element, bool recursive, const std::string& prefix)
{
	std::vector<std::string> values;

	if (!element)
		return values;

	std::vector<StaticPadTemplate> pad_templates = element->get_factory()->get_static_pad_templates();


	for(std::vector<StaticPadTemplate>::iterator iter = pad_templates.begin();
			iter != pad_templates.end(); ++iter )
		values.push_back(iter->get_name_template().c_str());

	if (recursive && GST_IS_BIN(element->gobj()))
	{
		RefPtr<Bin> bin = bin.cast_static(element);

		auto iterator = bin->iterate_elements();

		while (iterator.next())
		{
			if (GST_IS_BIN(iterator->gobj()))
			{
				auto ret = get_avaliable_pad_templates_string(*iterator, true, iterator->get_name().c_str() + std::string(":"));
				values.insert(values.end(), ret.begin(), ret.end());
			}
		}
	}

	return values;
}

bool GstUtils::is_specified_dir_element(const RefPtr<Element>& element, PadDirection dir)
{
	auto tpls = element->get_factory()->get_static_pad_templates();

	for (auto tpl : tpls)
	{
		if (tpl.get_direction() != dir)
			return false;
	}

	return true;
}
bool GstUtils::is_sink_element(const RefPtr<Element>& element)
{
	return is_specified_dir_element(element, PAD_SINK);
}
bool GstUtils::is_src_element(const RefPtr<Element>& element)
{
	return is_specified_dir_element(element, PAD_SRC);
}

std::string GstUtils::generate_element_path(RefPtr<Object> obj, const RefPtr<Object>& max_parent)
{
	std::string path;
	bool first = true;

	while (obj && obj != max_parent)
	{
		std::string tmp_path = obj->get_name().c_str();
		obj = obj->get_parent();
		if (!first)
			tmp_path += ":";
		else
			first = false;

		path = tmp_path + path;
	}

	return path;
}

void GstUtils::clean_model(const RefPtr<Pipeline>& model)
{
	auto iterator = model->iterate_elements();
	std::vector<RefPtr<Element>> elements;

	while (iterator.next())
		elements.push_back(*iterator);

	for (auto element : elements)
		model->remove(element);
}

Linkage GstUtils::find_connection(Glib::RefPtr<Gst::Pad> src_pad, Glib::RefPtr<Gst::Element> destination)
{
	auto second_iterator = destination->iterate_sink_pads();
	while (second_iterator.next())
	{
		if (src_pad->can_link(*second_iterator))
			return {true, src_pad, *second_iterator, src_pad->get_parent(), destination};
	}

	for (auto a : destination->get_factory()->get_static_pad_templates())
	{
		if (a.get_direction() == PAD_SRC || a.get_presence() == PAD_ALWAYS)
			continue;
		auto dest_tpl = destination->get_pad_template(a.get_name_template());
		if (src_pad->can_link(Gst::Pad::create(dest_tpl)))
			return {true, src_pad, dest_tpl, src_pad->get_parent(), destination};
	}

	return {false};
}

Linkage GstUtils::find_connection(Glib::RefPtr<Gst::Element> source, Glib::RefPtr<Gst::PadTemplate> dst_tpl)
{
	return find_connection(source, Pad::create(dst_tpl));
}

Linkage GstUtils::find_connection(Glib::RefPtr<Gst::PadTemplate> src_tpl, Glib::RefPtr<Gst::Element> destination)
{
	return find_connection(Pad::create(src_tpl), destination);
}

Linkage GstUtils::find_connection(Glib::RefPtr<Gst::Element> source, Glib::RefPtr<Gst::Pad> dst_port)
{
	auto iterator = source->iterate_src_pads();
	while (iterator.next())
	{
		if (iterator->can_link(dst_port))
			return {true, *iterator, dst_port, source, dst_port->get_parent()};
	}

	for (auto a : source->get_factory()->get_static_pad_templates())
	{
		if (a.get_direction() == PAD_SINK || a.get_presence() == PAD_ALWAYS)
			continue;
		auto src_tpl = source->get_pad_template(a.get_name_template());
		if (Gst::Pad::create(src_tpl)->can_link(dst_port))
			return {true, src_tpl, dst_port, source, dst_port->get_parent()};
	}

	return {false};
}

Linkage GstUtils::find_connection(Glib::RefPtr<Gst::Element> source, Glib::RefPtr<Gst::Element> destination)
{
	auto first_iterator = source->iterate_src_pads();
	while (first_iterator.next())
	{
		Linkage lnkg = find_connection(*first_iterator, destination);

		if (lnkg.exists)
			return lnkg;
	}

	for (auto fts : source->get_factory()->get_static_pad_templates())
	{
		if (fts.get_presence() == PAD_ALWAYS)
			continue;

		auto tpl = source->get_pad_template(fts.get_name_template());
		auto tmp_pad = Gst::Pad::create(tpl);
		auto second_iterator = destination->iterate_sink_pads();
		while (second_iterator.next())
		{
			if (tmp_pad->can_link(*second_iterator))
				return {true, tpl, *second_iterator, source, destination};
		}

		for (auto a : destination->get_factory()->get_static_pad_templates())
		{
			if (a.get_direction() == PAD_SRC || a.get_presence() == PAD_ALWAYS)
				continue;
			auto dest_tpl = destination->get_pad_template(a.get_name_template());
			if (tmp_pad->can_link(Gst::Pad::create(dest_tpl)))
				return {true, tpl, dest_tpl, source, destination};
		}
	}

	return {false};
}

Linkage GstUtils::find_connection(Glib::RefPtr<Gst::Object> source, Glib::RefPtr<Gst::Object> destination)
{
	if (GST_IS_ELEMENT(source->gobj()))
	{
		if (GST_IS_ELEMENT(destination->gobj()))
			return find_connection(RefPtr<Element>::cast_static(source), RefPtr<Element>::cast_static(destination));
		else if (GST_IS_PAD(destination->gobj()))
			return find_connection(RefPtr<Element>::cast_static(source), RefPtr<Pad>::cast_static(destination));
		else if (GST_IS_PAD_TEMPLATE(destination->gobj()))
			return find_connection(RefPtr<Element>::cast_static(source), RefPtr<PadTemplate>::cast_static(destination));
	}
	else if (GST_IS_ELEMENT(destination->gobj()))
	{
		if (GST_IS_ELEMENT(source->gobj()))
			return find_connection(RefPtr<Element>::cast_static(source), RefPtr<Element>::cast_static(destination));
		else if (GST_IS_PAD(source->gobj()))
			return find_connection(RefPtr<Pad>::cast_static(source), RefPtr<Element>::cast_static(destination));
		else if (GST_IS_PAD_TEMPLATE(source->gobj()))
			return find_connection(RefPtr<PadTemplate>::cast_static(source), RefPtr<Element>::cast_static(destination));
	}

	return {false};
}
