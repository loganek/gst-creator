/*
 * MainController.cpp
 *
 *  Created on: 4 gru 2013
 *      Author: Marcin Kolny
 */

#include "MainController.h"

using Glib::RefPtr;
using namespace Gst;

MainController::MainController(const RefPtr<Pipeline>& model)
: model(model)
{
}

RefPtr<Pipeline> MainController::get_model() const
{
	return model;
}


