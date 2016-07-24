/*
 * ReferenceForCpp.h
 *
 *  Created on: 24 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef REFERENCEFOCCPP_H_
#define REFERENCEFOCCPP_H_

#include "model/Reference.h"
#include "ExpressionForCpp.h"
#include "ReferenceForCpp.h"
#include "CompilableForCpp.h"
#include <string>
using std::string;

using Poco::FSM::MODEL::VariablePtr;
using Poco::FSM::MODEL::FunctionPtr;
using Poco::FSM::MODEL::LiteralPtr;
using Poco::FSM::MODEL::OperationPtr;

namespace Poco {
	namespace FSM {
		namespace PARSER {
			namespace CPP {

				class ReferenceForCpp : 
					public ExpressionForCpp, 
					public CompilableForCpp, 
					public Poco::FSM::MODEL::Reference {
				public:
					ReferenceForCpp(const VariablePtr variable, int lineno = 0):
						Reference(variable, lineno) {
					}
					ReferenceForCpp(const FunctionPtr function, int lineno = 0):
						Reference(function, lineno) {
					}
					ReferenceForCpp(const LiteralPtr literal, int lineno = 0):
						Reference(literal, lineno) {
					}
					virtual ~ReferenceForCpp() {
					}
					virtual void generateInclude(ostream& inc, bool debug) const;
					virtual void generateCode(ostream& cpp, bool debug) const;
					const string display(const string& prefix) const;
				};

			}
		}
	}
}

#endif
