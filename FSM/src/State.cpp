/*
 * State.cpp
 *
 *  Created on: 16 janv. 2016
 *      Author: FrancisANDRE
 */
#include <stdexcept>
#include "Poco/Format.h"
using Poco::format;


#include "model/State.h"
#include "model/Entry.h"
#include "model/Exit.h"
#include "model/Transition.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
TransitionPtr State::get(const string& name, const List<ParameterPtr>& parameters) const
{
    TransitionPtr transition = nullptr;
    string signature = Transition::signature(name, parameters);
    try
    {
        transition = maps.at(signature);
    }
    catch (std::out_of_range e)
    {
    }
    return transition;
}
TransitionPtr State::add(TransitionPtr transition)
{
    transition->begstate() = this;
    _transitions.push_back(transition);
    maps[transition->signature()] = transition;
    return transition;
}
void State::print(Print& print) const
{
    print(Element::name());
    if (entry())
        entry()->print(print);
    if (exit())
        exit()->print(print);
    print("{");
    for (const auto& transition : transitions())
    {
        transition->print(print);
    }
    print("}");
}
const string State::display() const
{
    if (oneline.empty())
    {
        oneline = name();
    }
    return oneline;
}
}
}
}