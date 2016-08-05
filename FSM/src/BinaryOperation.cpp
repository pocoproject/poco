//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#include "model/BinaryOperation.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
const string BinaryOperation::display() const
{
    if (_asstring.empty())
    {
        _asstring += _left->display();
        switch (_operator)
        {
        case equal:
            _asstring += "==";
            break;
        case notequal:
            _asstring += "!=";
            break;
        case and:
            _asstring += "&&";
            break;
        case or:
            _asstring += "||";
            break;
        case xor:
            _asstring += '^';
            break;
        default:
            _asstring += "???";
            break;
        }
        _asstring += _right->display();
    }
    return _asstring;
}
}
}
}