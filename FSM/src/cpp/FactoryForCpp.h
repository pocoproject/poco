/*
 * FactoryForCpp.h
 *
 *  Created on: 20 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_PARSER_CPP_FACTORY_H_
#define POCO_FSM_PARSER_CPP_FACTORY_H_
#include <string>
using std::string;


#include "model/SMC.h"
#include "model/Operation.h"
#include "model/Factory.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {
			namespace CPP {

				class FactoryForCpp : public Poco::FSM::MODEL::Factory {
				public:
					FactoryForCpp() {
					}
					virtual ~FactoryForCpp() {
					}
					FSMPtr				newFSM(Parser* parser) const;
					ActionPtr			newAction(const string& name, int lineno = 0) const;
					GuardPtr			newGuard(const string& name, int lineno = 0) const;
					MapPtr				newMap(const string& name, int lineno = 0) const;
					ParameterPtr		newParameter(const string& name, int lineno = 0) const;
					StatePtr			newState(const string& name, int lineno = 0) const;
					EntryPtr			newEntry(const string& name, int lineno = 0) const;
					ExitPtr				newExit(const string& name, int lineno = 0) const;
					TransitionPtr		newTransition(const string& name, int lineno = 0) const;
					VariablePtr			newVariable(const string& name, int lineno = 0) const;
					FunctionPtr			newFunction(const string& name, int lineno = 0) const;
					LiteralPtr			newLiteral(const string& name, int lineno = 0) const;
					ReferencePtr		newReference(const VariablePtr variable, int lineno = 0) const;
					ReferencePtr		newReference(const LiteralPtr literal, int lineno = 0) const;
					ReferencePtr		newReference(const FunctionPtr function, int lineno = 0) const;
					ArgumentPtr			newArgument(const string& name, int lineno = 0) const;
					UnaryOperationPtr	newUnaryOperation(Poco::FSM::MODEL::Operator op) const;
					BinaryOperationPtr	newBinaryOperation(Poco::FSM::MODEL::Operator op) const;
				};
			}
		}
	}
}
#endif
