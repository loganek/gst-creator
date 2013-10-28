/*
 * WorkspaceController.h
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef WORKSPACECONTROLLER_H_
#define WORKSPACECONTROLLER_H_

#include <gstreamermm.h>

class WorkspaceController
{
private:
	Glib::RefPtr<Gst::Pipeline> model;
public:
	WorkspaceController();
	virtual ~WorkspaceController();

	void add_element(const std::string& factory_name, const std::string& element_name);
	void remove_element(const std::string& element_name);
	void link_element(const std::string& source_element, const std::string& source_pad,
				const std::string& destination_element, const std::string& destination_pad);
	void unlink_element(const std::string& source_element, const std::string& source_pad,
				const std::string& destination_element, const std::string& destination_pad);
};

#endif /* WORKSPACECONTROLLER_H_ */
