/*
 * StateCommand.cpp
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#include "StateCommand.h"
#include "enum_string_converter.h"

StateCommand::StateCommand(State state, const Glib::RefPtr<Gst::Pipeline>& model)
: state(state),
  model(model)
{
}

StateCommand* StateCommand::from_args(const std::vector<std::string>& vect, const Glib::RefPtr<Gst::Pipeline>& model)
{
	if (vect.size() != 1)
		syntax_error("invalid arguments count. Expected 1, but " + std::to_string(vect.size()) + " found.");

	State state = string_to_enum<State>(vect[0]);

	return new StateCommand(state, model);
}

void StateCommand::run_command()
{
	Gst::State gst_state;

	switch (state)
	{
	case State::PLAY:
		gst_state = Gst::STATE_PLAYING;
		break;
	case State::PAUSE:
		gst_state = Gst::STATE_PAUSED;
		break;
	case State::STOP:
		gst_state = Gst::STATE_NULL;
		break;
	}

	model->set_state(gst_state);
}
