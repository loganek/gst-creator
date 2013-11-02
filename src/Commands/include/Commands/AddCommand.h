/*
 * AddCommand.h
 *
 *  Created on: 2 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef ADDCOMMAND_H_
#define ADDCOMMAND_H_

#include "Command.h"
#include <gstreamermm.h>

class AddCommand : public Command
{
private:
	Glib::RefPtr<Gst::Object> object;
	Glib::RefPtr<Gst::Element> parent;
	AddRemoveType type;

public:
	AddCommand(AddRemoveType type,
			const Glib::RefPtr<Gst::Element>& parent,
			const Glib::RefPtr<Gst::Object>& obj);
	virtual ~AddCommand();

	void run_command();
};

#endif /* ADDCOMMAND_H_ */
