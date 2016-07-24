/*
 * VariableForCpp.cpp
 *
 *  Created on: 21 janv. 2016
 *      Author: FrancisANDRE
 */

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
