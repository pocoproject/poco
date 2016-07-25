//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#include "model/Operation.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{

Operation::Operation(Operator op) :_operator(op)
{
}
Operation:: ~Operation()
{
}
void Operation::setNoContextuel()
{
    if (_operator == equal || _operator == notequal)
    {
        _contextuel = false;
    }
}
}
}
}