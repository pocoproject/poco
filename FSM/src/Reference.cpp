/*
 * Reference.cpp
 *
 *  Created on: 1 févr. 2016
 *      Author: FrancisANDRE
 */

#include "model/Reference.h"
#include "model/Guard.h"
#include "model/Transition.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
bool Reference::isParameter() const
{
    GuardPtr g = guard();
    TransitionPtr transition = guard()->transition();
    if (transition->hasParameter(reference()->Element::name()))
        return true;
    else
        return false;
}
const string Reference::display() const
{
    return reference()->display();
}
}
}
}
