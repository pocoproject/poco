/*
 * UnaryOperationForCpp.cpp
 *
 *  Created on: 24 janv. 2016
 *      Author: FrancisANDRE
 */

#include "cpp/UnaryOperationForCpp.h"

namespace Poco
{
namespace FSM
{
namespace PARSER
{
namespace CPP
{
const string UnaryOperationForCpp::display(const string& prefix) const
{
    string value;
    switch (_operator)
    {
    case Poco::FSM::MODEL::Operator::not:
        value += "!";
        break;
    default:
        value += "???";
        break;
    }
    value += dynamic_cast<ExpressionForCpp*>(operand())->display(prefix);
    return value;
}
}
}
}
}
