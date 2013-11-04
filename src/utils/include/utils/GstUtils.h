/*
 * GstUtils.h
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef GSTUTILS_H_
#define GSTUTILS_H_

#include <gstreamermm.h>

class GstUtils
{
public:
	static Glib::RefPtr<Gst::Element> find_element(std::string text, const Glib::RefPtr<Gst::Pipeline>& model);
};

#endif /* GSTUTILS_H_ */
