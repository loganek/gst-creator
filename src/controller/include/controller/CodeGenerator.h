/*
 * CodeGenerator.h
 *
 *  Created on: 6 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef CODEGENERATOR_H_
#define CODEGENERATOR_H_

#include <gstreamermm.h>
#include <string>
#include <fstream>

class CodeGenerator
{
	Glib::RefPtr<Gst::Pipeline> model;
	std::ofstream output;

	void generate_all();
	void generate_main();
	void generate_class();
	void generate_get_pipeline();
	void generate_create_pipeline();
	void generate_init_elements();
	void generate_link_static_pads();
public:
	CodeGenerator(const Glib::RefPtr<Gst::Pipeline>& model);

	void generate_code(const std::string& filename);
};

#endif /* CODEGENERATOR_H_ */
