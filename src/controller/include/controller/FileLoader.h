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
#include <functional>
#include "Commands.h"

class FileLoader
{
public:
	typedef std::function<void(const Glib::RefPtr<Gst::Element>&, double, double)> position_setter;
private:
	QXmlStreamReader reader;
	std::string filename;
	Glib::RefPtr<Gst::Pipeline> model;
	Glib::RefPtr<Gst::Element> current_element;
	std::stack<Glib::RefPtr<Gst::Element>> element_stack;
	std::map<Glib::ustring, Glib::ustring> connections;
	CommandListener* listener;
	QFile* file;
	position_setter pos_setter;

	Glib::ustring get_attribute(const char* attribute_name);
	void open_file();
	void clean_model();
	void process_start_element();
public:
	FileLoader(const std::string& filename, const Glib::RefPtr<Gst::Pipeline>& model, position_setter pos_setter);
	~FileLoader();

	void load_model(CommandListener* listener);
};


#endif /* FILELOADER_H_ */
