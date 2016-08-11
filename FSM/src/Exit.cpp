/*
 * Exit.cpp
 *
 *  Created on: 26 janv. 2016
 *      Author: FrancisANDRE
 */

#include "model/Exit.h"
#include "model/Action.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
const string Exit::display() const
{
    if (oneline.empty())
    {
        oneline += name();
    }
    return oneline;
}
void Exit::print(Print& print) const
{
    string line;
    line += "Exit\t{";

    for (const auto& action : actions())
        line += action->display();

    line += '}';
    print(line);
}
}
}
}
