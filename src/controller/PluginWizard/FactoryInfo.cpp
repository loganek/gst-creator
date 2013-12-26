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
  parent(parent), rank(rank), v_request_new_pad(false),
  v_release_pad(false), v_change_state(false),
  v_set_bus(false)
{
}

void FactoryInfo::set_virtual_methods(bool request_new_pad, bool release_pad, bool change_state, bool set_bus)
{
	v_request_new_pad = request_new_pad;
	v_release_pad = release_pad;
	v_change_state = change_state;
	v_set_bus = set_bus;
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
			<< "\t{" << endl << "\t\tGst::ElementFactory::register_element(plugin, \""
			<< StringUtils::to_lower(name) << "\", " << rank << ", Gst::register_mm_type<" << name
			<< ">(\"" << StringUtils::to_lower(name) << "\"));" << endl << "\treturn true;"
			<< endl << "\t}" << endl;

	if (v_release_pad)
		output << "\tvirtual void release_pad_vfunc(const Glib::RefPtr<Gst::Pad>& pad);" << endl;
	if (v_request_new_pad)
		output << "\tvirtual Glib::RefPtr<Gst::Pad> request_new_pad_vfunc(Glib::RefPtr<Gst::PadTemplate> templ, const Glib::ustring& name, const Glib::RefPtr<const Gst::Caps>& caps);" << endl;
	if (v_change_state)
		output << "\tvirtual Gst::StateChangeReturn change_state_vfunc(Gst::StateChange transition);" << endl;
	if (v_set_bus)
		output << "\tvirtual void set_bus_vfunc(const Glib::RefPtr<Gst::Bus>& bus);" << endl;

	output << "};" << endl << "#endif" << endl;


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

	if (v_release_pad)
		output << "void " << name << "::release_pad_vfunc(const Glib::RefPtr<Gst::Pad>& pad)" << endl
		<< "{" << endl << "\t" << parent << "::release_pad_vfunc(pad);" << endl << "}" << endl;
	if (v_request_new_pad)
		output << "Glib::RefPtr<Gst::Pad> " << name << "::request_new_pad_vfunc(Glib::RefPtr<Gst::PadTemplate> templ, const Glib::ustring& name, const Glib::RefPtr<const Gst::Caps>& caps)" << endl
		<< "{" << endl << "\treturn " << parent << "::request_new_pad_vfunc(templ, name, caps);" << endl << "}" << endl;
	if (v_change_state)
		output << "Gst::StateChangeReturn " << name << "::change_state_vfunc(Gst::StateChange transition)" << endl
		<< "{" << endl << "\treturn " << parent << "::change_state_vfunc(transition);" << endl << "}" << endl;
	if (v_set_bus)
		output << "void " << name << "::set_bus_vfunc(const Glib::RefPtr<Gst::Bus>& bus)" << endl
		<< "{" << endl << "\t" << parent << "::set_bus_vfunc(bus);" << endl << "}" << endl;

}

