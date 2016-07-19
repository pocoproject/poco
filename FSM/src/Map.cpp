/*
 * Map.cpp
 *
 *  Created on: 16 janv. 2016
 *      Author: FrancisANDRE
 */

#include "model/Map.h"
#include "model/Transition.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
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
    StatePtr result = nullptr;
    try
    {
        result = _states.at(state);
    }
    catch (const std::out_of_range&)
    {
    }
    return result;
}
void Map::print(Print& print) const
{
    for (const auto& state : states())
    {
        state.second->print(print);
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