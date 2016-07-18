/*
 * FactoryForCpp.cpp
 *
 *  Created on: 20 janv. 2016
 *      Author: FrancisANDRE
 */

#include "cpp/FactoryForCpp.h"
#include "cpp/ActionForCpp.h"
#include "cpp/FSMForCpp.h"
#include "cpp/GuardForCpp.h"
#include "cpp/MapForCpp.h"
#include "cpp/ParameterForCpp.h"
#include "cpp/StateForCpp.h"
#include "cpp/EntryForCpp.h"
#include "cpp/ExitForCpp.h"
#include "cpp/TransitionForCpp.h"
#include "cpp/ReferenceForCpp.h"
#include "cpp/VariableForCpp.h"
#include "cpp/FunctionForCpp.h"
#include "cpp/LiteralForCpp.h"
#include "cpp/ArgumentForCpp.h"
#include "cpp/UnaryOperationForCpp.h"
#include "cpp/BinaryOperationForCpp.h"


namespace Poco
{
namespace FSM
{
namespace PARSER
{
namespace CPP
{
ActionPtr		FactoryForCpp::newAction(const string& name, int lineno) const
{
    return new ActionForCpp(name, lineno);
}
FSMPtr			FactoryForCpp::newFSM(Parser* parser) const
{
    return new FSMForCpp(parser);
}
GuardPtr		FactoryForCpp::newGuard(const string& name, int lineno) const
{
    return new GuardForCpp(name, lineno);
}
MapPtr			FactoryForCpp::newMap(const string& name, int lineno) const
{
    return new MapForCpp(name, lineno);
}
ParameterPtr	FactoryForCpp::newParameter(const string& name, int lineno) const
{
    return new ParameterForCpp(name, lineno);
}
StatePtr		FactoryForCpp::newState(const string& name, int lineno) const
{
    return new StateForCpp(name, lineno);
}
EntryPtr		FactoryForCpp::newEntry(const string& name, int lineno) const
{
    return new EntryForCpp(name, lineno);
}
ExitPtr			FactoryForCpp::newExit(const string& name, int lineno) const
{
    return new ExitForCpp(name, lineno);
}
TransitionPtr	FactoryForCpp::newTransition(const string& name, int lineno) const
{
    return new TransitionForCpp(name, lineno);
}
ReferencePtr	FactoryForCpp::newReference(const VariablePtr variable, int lineno) const
{
    return new ReferenceForCpp(variable, lineno);
}
ReferencePtr	FactoryForCpp::newReference(const FunctionPtr function, int lineno) const
{
    return new ReferenceForCpp(function, lineno);
}
ReferencePtr	FactoryForCpp::newReference(const LiteralPtr literal, int lineno) const
{
    return new ReferenceForCpp(literal, lineno);
}
VariablePtr	FactoryForCpp::newVariable(const string& name, int lineno) const
{
    return new VariableForCpp(name, lineno);
}
FunctionPtr	FactoryForCpp::newFunction(const string& name, int lineno) const
{
    return new FunctionForCpp(name, lineno);
}
LiteralPtr	FactoryForCpp::newLiteral(const string& name, int lineno) const
{
    return new LiteralForCpp(name, lineno);
}
ArgumentPtr	FactoryForCpp::newArgument(const string& name, int lineno) const
{
    return new ArgumentForCpp(name, lineno);
}
UnaryOperationPtr	FactoryForCpp::newUnaryOperation(Poco::FSM::MODEL::Operator op) const
{
    return new UnaryOperationForCpp(op);
}
BinaryOperationPtr	FactoryForCpp::newBinaryOperation(Poco::FSM::MODEL::Operator op) const
{
    return new BinaryOperationForCpp(op);
}

}
}
}
}