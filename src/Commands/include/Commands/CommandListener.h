/*
 * CommandListener.h
 *
 *  Created on: 1 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef COMMANDLISTENER_H_
#define COMMANDLISTENER_H_

#include <gstreamermm.h>

class CommandListener
{
public:
	virtual void pad_added(const Glib::RefPtr<Gst::Pad>& pad) = 0;
	virtual void pad_removed(const Glib::RefPtr<Gst::Pad>& pad) = 0;
	virtual void pad_linked(const Glib::RefPtr<Gst::Pad>& first, const Glib::RefPtr<Gst::Pad>& second) = 0; // todo peer pad might be used
	virtual void pad_unlinked(const Glib::RefPtr<Gst::Pad>& pad) = 0;
	virtual ~CommandListener(){}
};

#endif /* COMMANDLISTENER_H_ */
