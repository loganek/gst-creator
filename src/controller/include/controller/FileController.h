/*
 * FileController.h
 *
 *  Created on: 27 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef FILECONTROLLER_H_
#define FILECONTROLLER_H_

#include <gstreamermm.h>
#include <QtCore>

class FileController
{
private:
	Glib::RefPtr<Gst::Pipeline> model;

	void write_single_element(QXmlStreamWriter* writer, const Glib::RefPtr<Gst::Element>& element);
public:
	FileController(const Glib::RefPtr<Gst::Pipeline>& model);

	Glib::RefPtr<Gst::Pipeline> get_model() const;

	void save_model(const std::string& filename);
	void load_model(const std::string& filename);
};

#endif /* FILECONTROLLER_H_ */
