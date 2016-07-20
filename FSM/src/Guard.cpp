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
Guard::Guard(const string& name, int lineno) : 
	Element(name, lineno),
	_transition(NULL),
	_condition(NULL),
	_endstate(NULL),
	_pushState(NULL),
	_popArgs(NULL)
{
}
Guard::~Guard() {
}

const string Guard::display() const
{
    if (oneline.empty())
    {
        string guards;
        if (_condition != NULL)
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
		List<ActionPtr>::const_iterator action;
		for (action = _actions.begin(); action != _actions.end(); ++action)
		{
            actions += (*action)->display();
        }
        actions += '}';

        oneline += actions;
    }
    return oneline;
}
}
}
}