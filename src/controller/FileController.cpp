/*
 * FileController.cpp
 *
 *  Created on: 27 lis 2013
 *      Author: Marcin Kolny
 */

#include "FileController.h"
#include "Properties/Property.h"

using namespace std;
using Glib::RefPtr;
using namespace Gst;

FileController::FileController(const RefPtr<Pipeline>& model)
: model(model)
{}

RefPtr<Pipeline> FileController::get_model() const
{
	return model;
}

void FileController::save_model(const std::string& filename)
{
	guint property_count;

	GParamSpec **property_specs = g_object_class_list_properties(
			G_OBJECT_GET_CLASS(model->gobj()), &property_count);

	QXmlStreamWriter* writer = new QXmlStreamWriter();

	QFile file(filename.c_str());
	if (!file.open(QIODevice::WriteOnly))
		throw runtime_error("Cannot open file " + filename + " for writting");

	writer->setDevice(&file);
	writer->writeStartDocument();
	writer->writeStartElement("pipeline");

	write_single_element(writer, model);

	writer->writeEndElement();
	writer->writeEndDocument();

	delete writer;
}

void FileController::write_single_element(QXmlStreamWriter* writer, const Glib::RefPtr<Gst::Element>& element)
{
	guint property_count;

	GParamSpec **property_specs = g_object_class_list_properties(
			G_OBJECT_GET_CLASS(element->gobj()), &property_count);
	writer->writeStartElement("element");

	for (int i = 0; i < property_count; i++)
	{
		Property* property = Property::build_property(property_specs[i], element, "");

		if (property == nullptr)
			continue;

		writer->writeStartElement("property");
		writer->writeAttribute("name", property_specs[i]->name);
		writer->writeCharacters(property->get_str_value().c_str());
		writer->writeEndElement();
		delete property;
	}

	if (GST_IS_BIN(element->gobj()))
	{
		Glib::RefPtr<Gst::Bin> bin = bin.cast_static(element);
		auto iterator = bin->iterate_elements();

		if (bin->get_num_children())
			writer->writeStartElement("children");
		while (iterator.next())
		{
			write_single_element(writer, *iterator);
		}
		if (bin->get_num_children())
			writer->writeEndElement();
	}

	writer->writeEndElement();
}

void FileController::load_model(const std::string& filename)
{
	// TODO
}


