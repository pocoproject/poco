//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

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
    case Poco::FSM::MODEL::equal:
        value += "==";
        break;
    case Poco::FSM::MODEL::notequal:
        value += "!=";
        break;
    case Poco::FSM::MODEL::and:
        value += "&&";
        break;
    case Poco::FSM::MODEL::or:
        value += "||";
        break;
    case Poco::FSM::MODEL::xor:
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
