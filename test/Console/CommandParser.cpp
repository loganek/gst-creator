/*
 * CommandParser.cpp
 *
 *  Created on: 31 pa? 2013
 *      Author: Marcin Kolny
 */

#include <gtest/gtest.h>
#include <gstreamermm.h>
#include "Console/CommandParser.h"

TEST(CommandParser, DummyTest)
{
	Glib::RefPtr<Gst::Pipeline> pipeline = Gst::Pipeline::create();
	CommandParser parser(pipeline);
	parser.parse("ADD xyz r r g");
	ASSERT_TRUE(true);
}


