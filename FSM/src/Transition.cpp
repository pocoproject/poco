/*
 * Transition.cpp
 *
 *  Created on: 16 janv. 2016
 *      Author: FrancisANDRE
 */

#include <algorithm>
using std::max;
using std::binary_search;

#include "Poco/Format.h"
using Poco::format;

#include "model/Transition.h"
#include "model/Parameter.h"
#include "model/Guard.h"

namespace Poco
{
namespace FSM
{
namespace MODEL
{
Transition::Transition(const string& name, int lineno) : 
	Element(name, lineno), _begstate(NULL)
{
}
Transition::~Transition()
{
}

bool parameterEqual(const Parameter* left, const Parameter* right)
{
    return left->Element::name() < right->Element::name();
}
bool Transition::hasParameter(const string& name) const
{
	List<ParameterPtr>::const_iterator parameter;
	for (parameter =_parameters.begin(); parameter != _parameters.end(); ++parameter)
    {
        if ((*parameter)->Element::name() == name)
            return true;
    }
    return false;
}
string Transition::declaration(const Parameter* added) const
{
    string declaration(name());
    declaration += '(';
    if (added != NULL)
    {
        declaration += added->type();
        declaration += ' ';
        declaration += added->name();
        if (!parameters().empty())
            declaration += ", ";
    }
    if (!parameters().empty())
    {
        for (int no = 0; no < parameters().size(); ++no)
        {
            declaration += parameters()[no]->type();
            declaration += ' ';
            declaration += parameters()[no]->name();
            if (no + 1 < parameters().size())
                declaration += ", ";
        }
    }
    declaration += ')';
    return declaration;
}
string Transition::call(const char* c) const
{
    string embed(name());
    embed += '(';
    if (c != NULL)
    {
        embed += c;
        if (!parameters().empty())
        {
            embed += ", ";
        }
    }
    if (!parameters().empty())
    {
        for (int no = 0; no < parameters().size(); ++no)
        {
            embed += parameters()[no]->name();
            if (no + 1 < parameters().size())
                embed += ", ";
        }
    }
    embed += ')';
    return embed;
}
string Transition::signature(const string& name, const List<ParameterPtr>& parameters)
{
    string signature(name);
    signature += '(';
    if (!parameters.empty())
    {
        for (int no = 0; no < parameters.size(); ++no)
        {
            signature += parameters[no]->type();
            if (no + 1 < parameters.size())
                signature += ", ";
        }
    }
    signature += ')';
    return signature;
}
const string& Transition::signature() const
{
    if (_signature.empty())
    {
        _signature = signature(Element::name(), _parameters);
    }
    return _signature;
}
const string Transition::display() const
{
    if (oneline.empty())
    {
        string transition = name();

        string parameters;
        if (!_parameters.empty())
            parameters += '(';
		List<ParameterPtr>::const_iterator parameter;
		for (parameter =_parameters.begin(); parameter != _parameters.end(); ++parameter)
        {
            parameters += (*parameter)->display();
        }
        if (!_parameters.empty())
            parameters += ')';

        transition += parameters;
        transition.resize(max(transition.size(), (size_t)30), ' ');

        string guards = _guards.at(0)->display();

        oneline = format("\t%s%s", transition, guards);
    }
    return oneline;
}
void Transition::print(Print& print) const
{
    List<GuardPtr>::const_iterator guard;
	for (guard = guards().begin(); guard != guards().end(); ++guard)
    {
        const TransitionPtr transitionptr = const_cast<TransitionPtr>(this);
        const GuardPtr guardptr = *guard;
        print(transitionptr, guardptr);
    }
}
}
}
}