//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#include "cpp/FunctionForCpp.h"

namespace Poco
{
namespace FSM
{
namespace PARSER
{
namespace CPP
{
const string FunctionForCpp::display(const string& prefix) const
{
    return prefix + Function::display();
}
}
}
}
}
