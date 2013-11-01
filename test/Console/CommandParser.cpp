/*
 * CommandParser.cpp
 *
 *  Created on: 31 pa? 2013
 *      Author: Marcin Kolny
 */

#include <gtest/gtest.h>
#include "Console/CommandParser.h"

TEST(CommandParser, DummyTest)
{
	CommandParser parser;
	parser.parse("add xyz");
	ASSERT_TRUE(true);
}


