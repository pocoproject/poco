//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#include <algorithm>
using std::max;

#include "Poco/Logger.h"
using Poco::Logger;

#include "model/Parameter.h"
#include "model/Guard.h"
#include "model/Transition.h"
#include "model/Print.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
void Print::operator()(const char* message)
{
    logger.information(message);
}
void Print::operator()(const string& message)
{
    logger.information(message);
}
void Print::operator()(const TransitionPtr transition, const GuardPtr guard)
{
    string line = transition->name();

    string parameters;
    if (!transition->parameters().empty())
        parameters += '(';
	List<ParameterPtr>::const_iterator parameter;
	for (parameter = transition->parameters().begin(); parameter != transition->parameters().end(); ++parameter)
    {
        parameters += (*parameter)->display();
    }
    if (!transition->parameters().empty())
        parameters += ')';

    line += parameters;
    line.resize(max(line.size(), (size_t)34), ' ');

    string guards = guard->display();
    logger.information("\t%s%s", line, guards);

}

}
}
}
