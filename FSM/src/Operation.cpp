/*
 * Operation.cpp
 *
 *  Created on: 20 janv. 2016
 *      Author: FrancisANDRE
 */

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
    if (_operator == Operator::equal || _operator == Operator::notequal)
    {
        _contextuel = false;
    }
}
}
}
}