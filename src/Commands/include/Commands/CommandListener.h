/*
 * CommandListener.h
 *
 *  Created on: 1 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef COMMANDLISTENER_H_
#define COMMANDLISTENER_H_

#include "ConnectCommand.h"
#include <gstreamermm.h>

class CommandListener
{
protected:
	static int refcount;
public:
	virtual void pad_added(const Glib::RefPtr<Gst::Pad>& pad) {}
	virtual void pad_removed(const Glib::RefPtr<Gst::Pad>& pad){}
	virtual void pad_linked(const Glib::RefPtr<Gst::Pad>& pad){}
	virtual void pad_unlinked(const Glib::RefPtr<Gst::Pad>& pad){}
	virtual void future_connection_added(const Glib::RefPtr<Gst::PadTemplate>& src_tpl,
			const Glib::RefPtr<Gst::Element>& parent, const Glib::RefPtr<Gst::Pad>& sink){}
	virtual void future_connection_removed(const ConnectCommand::future_connection_pads& conn){}
	virtual void state_changed(State state){}
	static int get_refcount() { return refcount; }
	virtual ~CommandListener(){}
};

#endif /* COMMANDLISTENER_H_ */
