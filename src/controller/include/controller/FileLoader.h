/*
 * FileLoader.h
 *
 *  Created on: 4 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef FILELOADER_H_
#define FILELOADER_H_

#include <stack>
#include <gstreamermm.h>
#include <QtCore>
#include "Commands.h"

class FileLoader
{
private:
	QXmlStreamReader reader;
	std::string filename;
	Glib::RefPtr<Gst::Pipeline> model;
	Glib::RefPtr<Gst::Element> current_element;
	std::stack<Glib::RefPtr<Gst::Element>> element_stack;
	std::map<Glib::ustring, Glib::ustring> connections;
	CommandListener* listener;
	QFile* file;

	Glib::ustring get_attribute(const char* attribute_name);
	void open_file();
	void clean_model();
	void process_start_element();
public:
	FileLoader(const std::string& filename, const Glib::RefPtr<Gst::Pipeline>& model);
	~FileLoader();

	void load_model(CommandListener* listener);
};


#endif /* FILELOADER_H_ */
