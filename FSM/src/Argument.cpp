/*
 * Argument.cpp
 *
 *  Created on: 21 janv. 2016
 *      Author: FrancisANDRE
 */

#include "model/Argument.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
const string Argument::display() const
{
    if (oneline.empty())
    {
        oneline += name();
    }
    return oneline;
}

}
}
}
