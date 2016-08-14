//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

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
