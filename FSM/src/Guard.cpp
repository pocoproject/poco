/*
 * Guard.cpp
 *
 *  Created on: 16 janv. 2016
 *      Author: FrancisANDRE
 */
#include <algorithm>
using std::max;


#include "model/Guard.h"
#include "model/Action.h"
#include "model/State.h"
#include "model/Expression.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
const string Guard::display() const
{
    if (oneline.empty())
    {
        string guards;
        if (_condition != nullptr)
        {
            guards += '[';
            guards += _condition->display();
            guards += ']';
        }
        guards.resize(max(guards.size(), (size_t)40), ' ');
        oneline += guards;

        string end = "nil";
        if (endstate())
            end = endstate()->name();
        end.resize(max(end.size(), (size_t)10), ' ');

        oneline += end;

        string actions;
        actions += '{';
        for (auto action : _actions)
        {
            actions += action->display();
        }
        actions += '}';

        oneline += actions;
    }
    return oneline;
}
}
}
}