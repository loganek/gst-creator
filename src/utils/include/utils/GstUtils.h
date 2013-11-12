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
public:
	static Glib::RefPtr<Gst::Element> find_element(std::string text, const Glib::RefPtr<Gst::Pipeline>& model);
	static Glib::RefPtr<Gst::Pad> find_pad(std::string text, const Glib::RefPtr<Gst::Pipeline>& model);
	static bool is_numeric_type(GType type);
	static std::vector<std::string> get_avaliable_elements_string();
	static std::vector<std::string> get_elements_from_bin_string(const Glib::RefPtr<Gst::Bin>& bin, bool must_bin, const std::string& prefix = "");
	static std::vector<std::string> get_properties_string(const Glib::RefPtr<Gst::Element>& element);
};

#endif /* GSTUTILS_H_ */
