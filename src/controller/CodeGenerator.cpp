/*
 * CodeGenerator.cpp
 *
 *  Created on: 6 gru 2013
 *      Author: Marcin Kolny
 */

#include "CodeGenerator.h"
#include <iostream>
#include "Commands/ConnectCommand.h"

using namespace std;
using namespace Gst;
using Glib::RefPtr;

CodeGenerator::CodeGenerator(const RefPtr<Pipeline>& model)
: model(model)
{
}

void CodeGenerator::generate_code(const string& filename)
{
	output.open(filename, std::ofstream::out | std::ofstream::trunc);

	generate_all();
}

void CodeGenerator::generate_all()
{
	output << "#include <stdexcept>" << endl;
	output << "#include <gstreamermm.h>" << endl;
	output << "#include <glibmm.h>" << endl << endl;

	generate_class();
	generate_get_pipeline();
	generate_create_pipeline();
	generate_init_elements();
	generate_static_links();
	generate_dynamic_links();
	generate_main();
}

void CodeGenerator::generate_main()
{
	output << endl << endl << "int main(int argc, char** argv)" << endl;
	output << "{" << endl;
	output << "\tGst::init(argc, argv);" << endl;
	output << "\tGlib::RefPtr<Gst::Pipeline> pipeline = Creator().get_pipeline();" << endl << endl;
	output << "\tif (!pipeline) throw std::runtime_error(\"Cannot create pipeline\");" << endl << endl;
	output << "\tpipeline->set_state(Gst::STATE_PLAYING);" << endl;
	output << "\tGlib::MainLoop::create()->run();" << endl;
	output << "\tpipeline->set_state(Gst::STATE_NULL);" << endl << endl;
	output << "\treturn 0;" << endl;
	output << "}" << endl;
}

void CodeGenerator::generate_class()
{
	output << "class Creator" << endl << "{" << endl;
	output << "private:" << endl;
	output << "\tGlib::RefPtr<Gst::Pipeline> pipeline;" << endl;

	auto iterator = model->iterate_elements();

	while (iterator.next())
	{
		output << "\tGlib::RefPtr<Gst::Element> "
				<< iterator->get_name() << ";" << endl;
	}
	output << endl << "\tvoid init_elements();" << endl;
	output << "\tvoid static_links();" << endl;
	output << "\tvoid dynamic_links();" << endl;
	output << "\tvoid create_pipeline();" << endl;
	output << endl << "public:" << endl;
	output << "\tvirtual ~Creator(){}" << endl;
	output << endl << "\tGlib::RefPtr<Gst::Pipeline> get_pipeline();" << endl;
	output << "};" << endl;
}

void CodeGenerator::generate_get_pipeline()
{
	output << "Glib::RefPtr<Gst::Pipeline> Creator::get_pipeline()" << endl;
	output << "{" << endl;
	output << "\tif (!pipeline)" << endl;
	output << "\t\tcreate_pipeline();" << endl;
	output << "\treturn pipeline;" << endl;
	output << "}" << endl;
}

void CodeGenerator::generate_create_pipeline()
{
	output << "void Creator::create_pipeline()" << endl << "{" << endl;
	output << "\tinit_elements();" << endl;
	output << "\tstatic_links();" << endl;
	output << "\tdynamic_links();" << endl;
	output << "}" << endl;
}

void CodeGenerator::generate_init_elements()
{
	output << "void Creator::init_elements()" << endl << "{" << endl;
	output << "\tpipeline = Gst::Pipeline::create(\""
			<< model->get_name() << "\");" << endl;

	auto iterator = model->iterate_elements();

	while (iterator.next())
	{
		output << "\t" << iterator->get_name() << " = "
				<< "Gst::ElementFactory::create_element(\""
				<< iterator->get_factory()->get_name() << "\", "
				"\"" << iterator->get_name() << "\");" << endl;
		output << "\tpipeline->add(" << iterator->get_name() << ");" << endl;

		auto pad_iterator = iterator->iterate_pads();

		while (pad_iterator.next())
		{
			if (pad_iterator->get_pad_template() &&
					pad_iterator->get_pad_template()->get_presence() == Gst::PAD_REQUEST)
			{
				output << "\t" << iterator->get_name() << "->request_pad("
						<< iterator->get_name() << "->get_pad_template(\""
						<< pad_iterator->get_pad_template()->get_name() << "\", \""
						<< pad_iterator->get_name() << "\");" << endl;
			}
		}
	}
	output << "}" << endl;
}

void CodeGenerator::generate_static_links()
{
	output << "void Creator::static_links()" << endl << "{" << endl;

	auto iterator = model->iterate_elements();

	while (iterator.next())
	{
		auto pad_iterator = iterator->iterate_src_pads();

		while (pad_iterator.next())
		{
			if (pad_iterator->is_linked())
			{
				output << "\t" << iterator->get_name() << "->link_pads(\""
						<< pad_iterator->get_name() << "\", "
						<< pad_iterator->get_peer()->get_parent_element()->get_name() << ", \""
						<< pad_iterator->get_peer()->get_name() << "\");" << endl;
			}
		}
	}

	output << "}" << endl;
}

void CodeGenerator::generate_dynamic_links()
{
	output << "void Creator::dynamic_links()" << endl << "{" << endl;

	auto future_connections = ConnectCommand::get_future_connections_pads();

	for (auto connection : future_connections)
	{
		output << "\t" << connection.first.first->get_name()
				<< "->signal_pad_added().connect("
				<< "[this](const Glib::RefPtr<Gst::Pad>& pad){" << endl
				<< "\t\tif (" << connection.second->get_parent_element()->get_name()
				<< "->get_static_pad(\"" << connection.second->get_name() << "\")"
				<< "->is_linked()) " << endl << "\t\t\treturn;" << endl
				<< "\t\tif (pad->get_pad_template()->get_name() == \""
				<< connection.first.second->get_name() << "\")"
				<< endl << "\t\t\tpad->link("
				<< connection.second->get_parent_element()->get_name()
				<< "->get_static_pad(\"" << connection.second->get_name()
				<< "\"));" << endl << "\t});" << endl << endl;

	}

	output << "}" << endl;
}


