//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#include "model/Print.h"
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
	List<ActionPtr>::const_iterator ci;

    for (ci = actions().begin(); ci !=actions().end(); ++ci)
        line += (*ci)->display();

    line += '}';
    print(line);
}
}
}
}
