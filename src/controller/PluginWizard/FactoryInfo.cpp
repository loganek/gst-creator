/*
 * FactoryInfo.cpp
 *
 *  Created on: 13 gru 2013
 *      Author: Marcin Kolny
 */

#include "PluginWizard/FactoryInfo.h"
#include "utils/StringUtils.h"
#include <fstream>
#include <stdexcept>

using namespace std;

FactoryInfo::FactoryInfo(string name, string long_name,
		string klass, string author,
		string description, string parent, int rank)
: name(name), long_name(long_name), klass(klass),
  author(author), description(description),
  parent(parent), rank(rank)
{
}

void FactoryInfo::generate_header(const string& path)
{
	string filename = path + "/" + name + ".h";
	ofstream output(filename, std::ofstream::out | std::ofstream::trunc);

	if (!output.is_open())
		throw runtime_error("Cannot open file " + filename + " for writing");

	output << "#ifndef " << StringUtils::to_upper(name) << "_H" << endl
			<< "#define " << StringUtils::to_upper(name) << "_H" << endl << endl
			<< "#include <gstreamermm.h> " << endl
			<< "#include <gstreamermm/private/element_p.h>" << endl
			<< "#include <glibmm.h>" << endl << endl
			<< "class " << name << " : public " << parent << endl
			<< "{" << endl << "private:" << endl
			<< "public:" << endl
			<< "\tstatic void base_init(Gst::ElementClass<" << name << "> *klass);" << endl
			<< "\texplicit " << name << "(" << parent << "::BaseObjectType* gobj);" << endl
			<< "\tstatic bool register_plugin(Glib::RefPtr<Gst::Plugin> plugin)" << endl
						<< "{" << endl << "\tGst::ElementFactory::register_element(plugin, \""
						<< StringUtils::to_lower(name) << "\", " << rank << ", Gst::register_mm_type<" << name
						<< ">(\"" << StringUtils::to_lower(name) << "\"));" << endl << "\treturn true;"
						<< endl << "}" << endl
			<< "};" << endl << "#endif" << endl;
}

void FactoryInfo::generate_source(const string& path)
{
	string filename = path + "/" + name + ".cpp";
	ofstream output(filename, std::ofstream::out | std::ofstream::trunc);

	if (!output.is_open())
		throw runtime_error("Cannot open file " + filename + " for writing");

	output << "#include \"" << name << ".h\"" << endl << endl
			<< "void " << name << "::base_init(Gst::ElementClass<" << name << "> *klass)"
			<< endl << "{" << endl << "\tklass->set_metadata(\"" << long_name << "\","
			<< "\"" << klass << "\", \"" << description << "\", \"" << author << "\");" << endl
			<< "}" << endl << endl
			<< name << "::" << name << "(" << parent << "::BaseObjectType* gobj)" << endl
			<< ": " << parent << "(gobj)" << endl << "{}" << endl << endl << endl;
}

