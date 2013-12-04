/*
 * MainController.h
 *
 *  Created on: 4 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef MAINCONTROLLER_H_
#define MAINCONTROLLER_H_

#include <gstreamermm.h>

class MainController
{
private:
	Glib::RefPtr<Gst::Pipeline> model;
public:
	MainController(const Glib::RefPtr<Gst::Pipeline>& model);
	Glib::RefPtr<Gst::Pipeline> get_model() const;
};

#endif /* MAINCONTROLLER_H_ */
