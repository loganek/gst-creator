/*
 * FileWriter.cpp
 *
 *  Created on: 4 gru 2013
 *      Author: Marcin Kolny
 */

#include "FileWriter.h"
#include "Properties/Property.h"
#include "utils/GstUtils.h"
#include "Commands/ConnectCommand.h"

using namespace std;
using Glib::RefPtr;
using namespace Gst;

FileWriter::FileWriter(const string& filename, const RefPtr<Pipeline>& model)
: model(model),
  filename(filename),
  file(nullptr)
{}

FileWriter::~FileWriter()
{
	delete file;
}

void FileWriter::open_file()
{
	delete file;
	file = new QFile(filename.c_str());

	if (!file->open(QIODevice::WriteOnly))
		throw runtime_error("Cannot open file " + filename + " for writting");

	writer.setDevice(file);
}

void FileWriter::save_model()
{
	open_file();

	writer.writeStartDocument();
	writer.writeStartElement("pipeline");

	write_single_element(model);
	write_future_connections();

	writer.writeEndElement();
	writer.writeEndDocument();
}

void FileWriter::write_single_element(const Glib::RefPtr<Gst::Element>& element)
{
	guint property_count;

	GParamSpec **property_specs = g_object_class_list_properties(
			G_OBJECT_GET_CLASS(element->gobj()), &property_count);

	for (int i = 0; i < property_count; i++)
	{
		Property* property = Property::build_property(property_specs[i], element, "");

		if (property == nullptr)
			continue;

		writer.writeStartElement("property");
		writer.writeAttribute("name", property_specs[i]->name);
		writer.writeCharacters(property->get_str_value().c_str());
		writer.writeEndElement();
		delete property;
	}

	auto pads = element->iterate_pads();

	while (pads.next())
	{
		writer.writeStartElement("pad");
		writer.writeAttribute("name", pads->get_name().c_str());
		if (pads->get_pad_template())
			writer.writeAttribute("template", pads->get_pad_template()->get_name().c_str());
		writer.writeAttribute("is_linked", std::to_string(pads->is_linked()).c_str());
		if (pads->is_linked())
			writer.writeCharacters((pads->get_peer()->get_parent_element()->get_name() + ":" + pads->get_peer()->get_name()).c_str());
		writer.writeEndElement();
	}

	if (GST_IS_BIN(element->gobj()) && element == model) // TODO deeper search in a next version
	{
		Glib::RefPtr<Gst::Bin> bin = bin.cast_static(element);
		auto iterator = bin->iterate_elements();

		if (bin->get_num_children())
			writer.writeStartElement("children");
		while (iterator.next())
		{
			writer.writeStartElement("element");
			writer.writeAttribute("factory", iterator->get_factory()->get_name().c_str());
			writer.writeAttribute("name", iterator->get_name().c_str());
			write_single_element(*iterator);
			writer.writeEndElement();
		}
		if (bin->get_num_children())
			writer.writeEndElement();
	}
}

void FileWriter::write_future_connections()
{
	for (auto connection_element : ConnectCommand::get_future_connections_element())
	{
		writer.writeStartElement("future-connection");
		writer.writeAttribute("type", "element");
		writer.writeAttribute("source", GstUtils::generate_element_path(connection_element.first, model).c_str());
		writer.writeAttribute("destination", GstUtils::generate_element_path(connection_element.second, model).c_str());
		writer.writeEndElement();
	}

	for (auto connection_pad : ConnectCommand::get_future_connections_pads())
	{
		writer.writeStartElement("future-connection");
		writer.writeAttribute("type", "pad");
		writer.writeAttribute("template-parent", GstUtils::generate_element_path(connection_pad.first.first, model).c_str());
		writer.writeAttribute("template", connection_pad.first.second->get_name().c_str());
		writer.writeAttribute("destination", GstUtils::generate_element_path(connection_pad.second, model).c_str());
		writer.writeEndElement();
	}
}
