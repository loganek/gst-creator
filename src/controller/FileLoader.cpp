/*
 * FileLoader.cpp
 *
 *  Created on: 4 gru 2013
 *      Author: Marcin Kolny
 */

#include "FileLoader.h"
#include "utils/GstUtils.h"

using namespace std;
using namespace Gst;
using Glib::RefPtr;

FileLoader::FileLoader(const string& filename, const RefPtr<Pipeline>& model)
: filename(filename),
  model(model),
  listener(nullptr),
  file(nullptr)
{}

FileLoader::~FileLoader()
{
	delete file;
}

void FileLoader::open_file()
{
	delete file;
	file = new QFile(filename.c_str());

	if (!file->open(QIODevice::ReadOnly))
		throw runtime_error("Cannot open file " + filename + " for writting");

	reader.setDevice(file);
}

void FileLoader::clean_model()
{
	auto iterator = model->iterate_elements();
	vector<RefPtr<Element>> elements;

	while (iterator.next())
		elements.push_back(*iterator);

	for (auto element : elements)
		model->remove(element);
}

void FileLoader::load_model(CommandListener* listener)
{
	clean_model();
	open_file();

	this->listener = listener;

	while (!reader.atEnd() && !reader.hasError())
	{
		QXmlStreamReader::TokenType token = reader.readNext();

		if (token == QXmlStreamReader::StartDocument)
			continue;

		if (token == QXmlStreamReader::StartElement)
		{
			process_start_element();
		}
		else if (token == QXmlStreamReader::EndElement)
		{
			if (reader.name() == "element")
			{
				current_element = element_stack.top();
				element_stack.pop();
			}
		}
	}

	if (reader.hasError())
		throw std::runtime_error(reader.errorString().toUtf8().constData());

	for (auto con : connections)
	{
		RefPtr<Pad> src_pad = GstUtils::find_pad(con.first.c_str(), model);
		RefPtr<Pad> sink_pad = GstUtils::find_pad(con.second.c_str(), model);

		if (src_pad && sink_pad)
		{
			ConnectCommand cmd(src_pad, sink_pad);
			cmd.run_command(listener);
		}
	}
}

Glib::ustring FileLoader::get_attribute(const char* attribute_name)
{
	return (reader.attributes().hasAttribute(attribute_name)) ?
			reader.attributes().value(attribute_name).toUtf8().constData() :
			Glib::ustring();
}

void FileLoader::process_start_element()
{
	if (reader.name() == "pipeline")
		current_element = model;
	else if (reader.name() == "element" && reader.attributes().hasAttribute("factory"))
	{
		RefPtr<Element> new_element =
				(reader.attributes().hasAttribute("name")) ?
						ElementFactory::create_element(get_attribute("factory"),
								get_attribute("name")) :
								ElementFactory::create_element(get_attribute("factory"));

		AddCommand cmd(ObjectType::ELEMENT, current_element, new_element);
		cmd.run_command(listener);

		element_stack.push(current_element);
		current_element = new_element;
	}
	else if (reader.name() == "property")
	{
		Glib::ustring spec_name = get_attribute("name");
		if (spec_name.empty()) return;

		GParamSpec* spec = g_object_class_find_property(
				G_OBJECT_GET_CLASS(current_element->gobj()), spec_name.c_str());

		if (spec == nullptr) return;

		if (reader.readNext() == QXmlStreamReader::Characters)
		{
			Property* prop = Property::build_property(spec, current_element,
					reader.text().toString().toUtf8().constData());
			if (prop)
				prop->set_value();
		}
	}
	else if (reader.name() == "pad")
	{
		Glib::ustring pad_name = get_attribute("name"),
				pad_template_name = get_attribute("template"),
				pad_is_linked = get_attribute("is_linked");

		if (pad_name.empty() || pad_template_name.empty() || pad_is_linked.empty())
			return;

		bool is_linked = pad_is_linked == "1";

		RefPtr<PadTemplate> pad_template = current_element->get_pad_template(pad_template_name);

		if (!current_element->get_static_pad(pad_name))
		{
			AddCommand cmd(ObjectType::PAD, current_element,
					Pad::create(pad_template, pad_name));
			cmd.run_command(listener);
		}

		if (is_linked && pad_template->get_direction() == PAD_SRC)
		{
			if (reader.readNext() != QXmlStreamReader::Characters) return;

			Glib::ustring sink_pad_text = reader.text().toString().toUtf8().constData();
			Glib::ustring current_name = current_element->get_name() + ":" + pad_name;
			connections[current_name] = sink_pad_text;
		}
	}
	else if (reader.name() == "future-connection")
	{
		Glib::ustring connection_type = get_attribute("type");

		if (connection_type == "element")
		{
			Glib::ustring source = get_attribute("source"),
					destination = get_attribute("destination");

			if (source.empty() || destination.empty())
				return;

			RefPtr<Element> source_e = GstUtils::find_element(source.c_str(), model);
			RefPtr<Element> destination_e = GstUtils::find_element(destination.c_str(), model);

			if (!source_e || !destination_e)
				return;

			ConnectCommand cmd(source_e, destination_e, true);
			cmd.run_command(listener);
		}
		else if (connection_type == "pad")
		{
			Glib::ustring template_parent = get_attribute("template-parent"),
					tpl = get_attribute("template"),
					destination = get_attribute("destination");

			if (template_parent.empty() || tpl.empty() || destination.empty())
				return;

			RefPtr<Pad> dest_pad = GstUtils::find_pad(destination.c_str(), model);
			RefPtr<Element> tpl_parent = GstUtils::find_element(template_parent.c_str(), model);

			if (!dest_pad || !tpl_parent)
				return;

			RefPtr<PadTemplate> pad_tpl = tpl_parent->get_pad_template(tpl);

			if (!pad_tpl)
				return;

			ConnectCommand cmd(pad_tpl, tpl_parent, dest_pad);
			cmd.run_command(listener);
		}
	}
}
