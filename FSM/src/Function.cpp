//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#include "model/Function.h"
#include "model/Argument.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
const string Function::display() const
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
    }
    return oneline;
}

}
}
}