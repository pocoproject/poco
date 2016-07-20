//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#include "model/Print.h"
#include "model/Entry.h"
#include "model/Action.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
const string Entry::display() const
{
    if (oneline.empty())
    {
        oneline += Element::name();
    }
    return oneline;
}
void Entry::print(Print& print) const
{
    string line;
    line += "Entry\t{";
	List<ActionPtr>::const_iterator ci;

	for (ci = actions().begin(); ci !=actions().end(); ++ci)
		line += (*ci)->display();

    line += '}';
    print(line);
}
}
}
}
