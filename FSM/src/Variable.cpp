/*
 * Variable.cpp
 *
 *  Created on: 21 janv. 2016
 *      Author: FrancisANDRE
 */

#include "model/Variable.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
const string Variable::display() const
{
    return name();
}
}
}
}