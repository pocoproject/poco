//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#include "cpp/VariableForCpp.h"

using namespace Poco::FSM::MODEL;

namespace Poco
{
namespace FSM
{
namespace PARSER
{
namespace CPP
{
const string VariableForCpp::display(const string& prefix) const
{
    return prefix + name();
}
}
}
}
}
