/*
 * Factory.h
 *
 *  Created on: 26 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_MODEL_FACTORY_H_
#define POCO_FSM_MODEL_FACTORY_H_

#include "SMC.h"
#include "Operation.h"

using Poco::FSM::MODEL::ActionPtr;
using Poco::FSM::MODEL::FSMPtr;
using Poco::FSM::MODEL::GuardPtr;
using Poco::FSM::MODEL::MapPtr;
using Poco::FSM::MODEL::ParameterPtr;
using Poco::FSM::MODEL::StatePtr;
using Poco::FSM::MODEL::EntryPtr;
using Poco::FSM::MODEL::ExitPtr;
using Poco::FSM::MODEL::TransitionPtr;
using Poco::FSM::MODEL::ExpressionPtr;
using Poco::FSM::MODEL::ReferencePtr;
using Poco::FSM::MODEL::VariablePtr;
using Poco::FSM::MODEL::FunctionPtr;
using Poco::FSM::MODEL::LiteralPtr;
using Poco::FSM::MODEL::ArgumentPtr;
using Poco::FSM::MODEL::OperationPtr;
using Poco::FSM::MODEL::UnaryOperationPtr;
using Poco::FSM::MODEL::BinaryOperationPtr;

namespace Poco {
	namespace FSM {
		namespace MODEL {

			class Factory {
			public:
				Factory() {
				}
				virtual ~Factory() {
				}
				virtual FSMPtr				newFSM(Parser* parser) const = 0;
				virtual ActionPtr			newAction(const string& name, int lineno = 0) const = 0;
				virtual GuardPtr			newGuard(const string& name, int lineno = 0) const = 0;
				virtual MapPtr				newMap(const string& name, int lineno = 0) const = 0;
				virtual ParameterPtr		newParameter(const string& name, int lineno = 0) const = 0;
				virtual StatePtr			newState(const string& name, int lineno = 0) const = 0;
				virtual EntryPtr			newEntry(const string& name, int lineno = 0) const = 0;
				virtual ExitPtr				newExit(const string& name, int lineno = 0) const = 0;
				virtual TransitionPtr		newTransition(const string& name, int lineno = 0) const = 0;
				virtual VariablePtr			newVariable(const string& name, int lineno = 0) const = 0;
				virtual FunctionPtr			newFunction(const string& name, int lineno = 0) const = 0;
				virtual LiteralPtr			newLiteral(const string& name, int lineno = 0) const = 0;
				virtual	ReferencePtr		newReference(const VariablePtr variable, int lineno = 0) const = 0;
				virtual	ReferencePtr		newReference(const LiteralPtr literal, int lineno = 0) const = 0;
				virtual	ReferencePtr		newReference(const FunctionPtr function, int lineno = 0) const = 0;
				virtual ArgumentPtr			newArgument(const string& name, int lineno = 0) const = 0;
				virtual UnaryOperationPtr	newUnaryOperation(Operator op) const = 0;
				virtual BinaryOperationPtr	newBinaryOperation(Operator op) const = 0;
			};
		}
	}
}

#endif
