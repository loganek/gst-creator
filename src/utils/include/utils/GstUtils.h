/*
 * GstUtils.h
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef GSTUTILS_H_
#define GSTUTILS_H_

#include <gstreamermm.h>
#include <vector>

class GstUtils
{
private:
	static bool is_specified_dir_element(const Glib::RefPtr<Gst::Element>& element, Gst::PadDirection dir);
public:
	static Glib::RefPtr<Gst::Element> find_element(std::string text, const Glib::RefPtr<Gst::Pipeline>& model);
	static Glib::RefPtr<Gst::Pad> find_pad(std::string text, const Glib::RefPtr<Gst::Pipeline>& model);
	static Glib::RefPtr<Gst::PadTemplate> find_pad_template(std::string text, const Glib::RefPtr<Gst::Pipeline>& model);
	static bool is_numeric_type(GType type);
	static std::vector<std::string> get_avaliable_elements_string();
	static std::vector<std::string> get_elements_from_bin_string(const Glib::RefPtr<Gst::Bin>& bin, bool must_bin, const std::string& prefix = "");
	static std::vector<std::string> get_properties_string(const Glib::RefPtr<Gst::Element>& element);
	static std::vector<std::string> get_avaliable_pad_templates_string(const Glib::RefPtr<Gst::Element>& element, bool recursive = false, const std::string& prefix = "");
	static std::vector<std::string> get_pads_from_bin_string(const Glib::RefPtr<Gst::Bin>& bin, const std::string& prefix = "");
	static std::vector<std::string> get_pads_from_element_string(const Glib::RefPtr<Gst::Element>& element);
	static bool is_sink_element(const Glib::RefPtr<Gst::Element>& element);
	static bool is_src_element(const Glib::RefPtr<Gst::Element>& element);
};

#endif /* GSTUTILS_H_ */
