//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#include "model/Map.h"
#include "model/Transition.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
Map::Map(const string& name, int lineno) : 
	Element(name, lineno), _fsm(NULL), _defaultState(NULL) {
}
Map::~Map() {
}
void Map::add(TransitionPtr transition)
{
    if (transition->name() != "Default")
        _transitions[transition->signature()] = transition;
}

void Map::add(StatePtr state)
{
    _states[state->name()] = state;
    state->map() = this;
}

StatePtr Map::lookfor(const string& state) const
{
    StatePtr result = NULL;
    try
    {
        result = _states.at(state);
    }
    catch (std::out_of_range oor)
    {
    }
    return result;
}
void Map::print(Print& print) const
{
	map<string, StatePtr>::const_iterator state;
	for (state = states().begin(); state != states().end(); ++state)
    {
        state->second->print(print);
    }
}
const string Map::display() const
{
    if (oneline.empty())
    {

    }
    return oneline;
}
}
}
}