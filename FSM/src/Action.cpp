/*
 * Action.cpp
 *
 *  Created on: 16 janv. 2016
 *      Author: FrancisANDRE
 */

#include "model/Action.h"
#include "model/Argument.h"
#include "model/Print.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
const string Action::display() const
{
    if (oneline.empty())
    {
        oneline += name();
        oneline += '(';
        string arguments;

        for (int no = 0; no < _arguments.size(); ++no)
        {
            arguments += _arguments[no]->name();
            if (no + 1 < _arguments.size())
                arguments += ", ";
        }
        oneline += arguments;
        oneline += ')';
        oneline += ';';
    }
    return oneline;
}
void Action::print(Print& print) const
{
    print(display());
}
}
}
}
