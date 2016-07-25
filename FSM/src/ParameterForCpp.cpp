//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#include <ostream>
#include "cpp/ParameterForCpp.h"
using namespace std;

namespace Poco
{
namespace FSM
{
namespace PARSER
{
namespace CPP
{
void ParameterForCpp::generateInclude(ostream& inc, bool debug) const
{
    inc << type() << ' ' << name();
}
void ParameterForCpp::generateCode(ostream& cpp, bool debug) const
{
    cpp << type() << ' ' << name();
}
}
}
}
}