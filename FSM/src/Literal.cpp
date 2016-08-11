/*
 * Literal.cpp
 *
 *  Created on: 21 janv. 2016
 *      Author: FrancisANDRE
 */

#include "model/Literal.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
const string Literal::display() const
{
    return name();
}
const string Literal::display(const string& prefix) const
{
    return display();
}
}
}
}
