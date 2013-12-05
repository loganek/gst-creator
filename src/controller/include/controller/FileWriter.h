/*
 * FileWriter.h
 *
 *  Created on: 4 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef FILEWRITER_H_
#define FILEWRITER_H_

#include <gstreamermm.h>
#include <QtCore>

class FileWriter
{
private:
	Glib::RefPtr<Gst::Pipeline> model;
	std::string filename;
	QXmlStreamWriter writer;
	QFile* file;

	void write_single_element(const Glib::RefPtr<Gst::Element>& element);
	void write_future_connections();
	void open_file();
public:
	FileWriter(const std::string& filename, const Glib::RefPtr<Gst::Pipeline>& model);
	virtual ~FileWriter();

	void save_model();
};

#endif /* FILEWRITER_H_ */
