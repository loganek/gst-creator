/*
 * PluginInfo.cpp
 *
 *  Created on: 13 gru 2013
 *      Author: Marcin Kolny
 */

#include "PluginWizard/PluginInfo.h"
#include <fstream>
#include <stdexcept>

using namespace std;

PluginInfo::PluginInfo(string name, string description,
		string version, string license,
		string source_module, string binary_package,
		string origin_url)
: name(name), description(description),
  version(version), license(license),
  source_module(source_module), binary_package(binary_package),
  origin_url(origin_url)
{}

void PluginInfo::generate_code(const vector<string>& names, const string& path)
{
	string filename = path + "/" + name + ".cpp";
	ofstream output(filename, std::ofstream::out | std::ofstream::trunc);

	if (!output.is_open())
		throw runtime_error("Cannot open file " + filename + " for writing");

	for (auto name : names)
		output << "#include \"" << name << ".h\"" << endl;

	output << "#include <gstreamermm.h>" << endl
			<< "static gboolean plugin_init(GstPlugin* plugin)" << endl
			<< "{" << endl << "\tint argc = 0; char** argv = nullptr;" << endl
			<< "\tGst::init(argc, argv);" << endl;

	for (auto name : names)
		output << "\tif (!" << name << "::register_plugin(Glib::wrap(plugin, true)))" << endl
				<< "\t\treturn false;" << endl;

	output << "\treturn true;" << endl << "}" << endl << "#define PACKAGE \"" << source_module
			<< "\"" << endl << "GST_PLUGIN_DEFINE (" << endl
			<< "\tGST_VERSION_MAJOR," << endl << "\tGST_VERSION_MINOR," << endl
			<< "\t" << name << "," << endl << "\t\"" << description << "\"," << endl
			<< "\tplugin_init," << endl << "\t\"" << version << "\","
			<< endl << "\t\"" << license << "\","
			<< endl << "\t\"" << binary_package << "\","
			<< endl << "\t\"" << origin_url << "\"" << endl << ")" << endl;
}
