//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#include "model/Literal.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
const string Literal::display() const
{
    return name();
}
const string Literal::display(const string& prefix) const
{
    return display();
}
}
}
}
