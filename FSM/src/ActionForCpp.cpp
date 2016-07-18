/*
 * ActionForCpp.cpp
 *
 *  Created on: 19 janv. 2016
 *      Author: FrancisANDRE
 */

#include <algorithm>
using std::replace;

#include "cpp/ActionForCpp.h"

namespace Poco
{
namespace FSM
{
namespace PARSER
{
namespace CPP
{
const string ActionForCpp::name() const
{
    string cppname = Element::name();
    replace(cppname.begin(), cppname.end(), '-', '_');
    return cppname;
}
void ActionForCpp::generateInclude(ostream& inc, bool debug) const
{
}
void ActionForCpp::generateCode(ostream& cpp, bool debug) const
{
}
}
}
}
}
