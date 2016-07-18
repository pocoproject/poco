/*
 * FunctionForCpp.cpp
 *
 *  Created on: 21 janv. 2016
 *      Author: FrancisANDRE
 */

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
