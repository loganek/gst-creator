/*
 * FileController.cpp
 *
 *  Created on: 27 lis 2013
 *      Author: Marcin Kolny
 */

#include "FileController.h"
#include "Properties/Property.h"
#include <QMessageBox>
#include <stack>

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

	auto pads = element->iterate_pads();

	while (pads.next())
	{
		writer->writeStartElement("pad");
		writer->writeAttribute("name", pads->get_name().c_str());
		writer->writeAttribute("template", pads->get_pad_template()->get_name().c_str());
		writer->writeAttribute("is_linked", std::to_string(pads->is_linked()).c_str());
		if (pads->is_linked())
			writer->writeCharacters((pads->get_peer()->get_parent_element()->get_name() + ":" + pads->get_peer()->get_name()).c_str());
		writer->writeEndElement();
	}

	if (GST_IS_BIN(element->gobj()))
	{
		Glib::RefPtr<Gst::Bin> bin = bin.cast_static(element);
		auto iterator = bin->iterate_elements();

		if (bin->get_num_children())
			writer->writeStartElement("children");
		while (iterator.next())
		{
			writer->writeStartElement("element");
			writer->writeAttribute("factory", iterator->get_factory()->get_name().c_str());
			write_single_element(writer, *iterator);
			writer->writeEndElement();
		}
		if (bin->get_num_children())
			writer->writeEndElement();
	}
}

void FileController::load_model(const std::string& filename, CommandListener* listener)
{
	auto iterator = model->iterate_elements();

	while (iterator.next())
		model->remove(*iterator);

	QXmlStreamReader* reader = new QXmlStreamReader();

	int state = 0;

	QFile file(filename.c_str());
	if (!file.open(QIODevice::ReadOnly))
		throw runtime_error("Cannot open file " + filename + " for writting");

	reader->setDevice(&file);
	RefPtr<Element> current_element;
	stack<RefPtr<Element>> element_stack;
	while (!reader->atEnd() && !reader->hasError())
	{
		QXmlStreamReader::TokenType token = reader->readNext();

		if (token == QXmlStreamReader::StartDocument)
			continue;

		if (token == QXmlStreamReader::StartElement)
		{
			if (reader->name() == "pipeline")
				current_element = model;
			else if (reader->name() == "element")
			{
				if (reader->attributes().hasAttribute("factory"))
				{
					RefPtr<Element> new_element = ElementFactory::create_element(reader->attributes().value("factory").toUtf8().constData());
					AddCommand cmd(ObjectType::ELEMENT, current_element, new_element);
					cmd.run_command(listener);
					element_stack.push(current_element);
					current_element = new_element;
				}

			}
			else if (reader->name() == "property")
			{
				if (reader->attributes().hasAttribute("name"))
				{
					GParamSpec* spec = g_object_class_find_property(G_OBJECT_GET_CLASS(current_element->gobj()),
							reader->attributes().value("name").toUtf8().constData());
					if (spec != nullptr)
					{
						reader->readNext();
						qDebug() << "Laduje propa: " << reader->text().toString();
						string text;
						if (reader->tokenType() == QXmlStreamReader::Characters)
							text = reader->text().toString().toUtf8().constData();

						Property* prop = Property::build_property(spec, current_element, text);
						if (prop)
							prop->set_value();
					}
				}
			}
		}
		else if (token == QXmlStreamReader::EndElement)
		{
			if (reader->name() == "element")
			{
				current_element = element_stack.top();
				element_stack.pop();
			}
		}
	}
	if (reader->hasError())
		qDebug() << reader->errorString();

	delete reader;
}


