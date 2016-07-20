//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#include "model/Parameter.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
const string Parameter::display() const
{
    if (oneline.empty())
    {
        oneline += name();
        oneline += ':';
        oneline += _type;
    }
    return oneline;
}
}
}
}