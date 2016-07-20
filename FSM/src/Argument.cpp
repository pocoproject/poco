//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#include "model/Argument.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
const string Argument::display() const
{
    if (oneline.empty())
    {
        oneline += name();
    }
    return oneline;
}

}
}
}
