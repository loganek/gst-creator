/*
 * StateCommand.cpp
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#include "StateCommand.h"
#include "CommandListener.h"
#include "utils/EnumUtils.h"

using Glib::RefPtr;
using namespace std;

StateCommand::StateCommand(State state, const RefPtr<Gst::Pipeline>& model)
: Command(CommandType::STATE),
  state(state),
  model(model)
{
}

StateCommand* StateCommand::from_args(const std::vector<std::string>& vect, const RefPtr<Gst::Pipeline>& model)
{
	if (vect.size() != 1)
		syntax_error("invalid arguments count. Expected 1, but " + to_string(vect.size()) + " found.");

	State state = EnumUtils<State>::string_to_enum(vect[0]);

	return new StateCommand(state, model);
}

void StateCommand::run_command(std::vector<CommandListener*> listeners)
{
	Gst::State gst_state = Gst::STATE_NULL;

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

	for (auto listener : listeners)
		listener->state_changed(state);

	model->set_state(gst_state);
}

vector<string> StateCommand::get_suggestions(const vector<string>& args, const RefPtr<Gst::Pipeline>& model)
{
	if (args.size() == 1)
			return EnumUtils<State>::get_string_values();

	return vector<string>();
}
