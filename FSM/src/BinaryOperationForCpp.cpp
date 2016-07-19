/*
 * BinaryOperationForCpp.cpp
 *
 *  Created on: 24 janv. 2016
 *      Author: FrancisANDRE
 */

#include "cpp/BinaryOperationForCpp.h"

namespace Poco
{
namespace FSM
{
namespace PARSER
{
namespace CPP
{
const string BinaryOperationForCpp::display(const string& prefix) const
{
    string value;
    value += dynamic_cast<ExpressionForCpp*>(left())->display(prefix);
    switch (_operator)
    {
    case Poco::FSM::MODEL::Operator::equal:
        value += "==";
        break;
    case Poco::FSM::MODEL::Operator::notequal:
        value += "!=";
        break;
    case Poco::FSM::MODEL::Operator::and:
        value += "&&";
        break;
    case Poco::FSM::MODEL::Operator::or:
        value += "||";
        break;
    case Poco::FSM::MODEL::Operator::xor:
        value += '^';
        break;
    default:
        value += "???";
        break;
    }
    value += dynamic_cast<ExpressionForCpp*>(right())->display(prefix);
    return value;
}

}
}
}
}
