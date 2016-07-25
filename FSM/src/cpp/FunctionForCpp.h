//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#ifndef FUNCTIONFORCPP_H_
#define FUNCTIONFORCPP_H_

#include "model/Function.h"
#include "CompilableForCpp.h"
#include "ReferenceForCpp.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {
			namespace CPP {

				class FunctionForCpp : public FSM::MODEL::Function {
				public:
					FunctionForCpp(const string& name, int lineno = 0) :
						FSM::MODEL::Function(name, lineno) {
					}
					virtual ~FunctionForCpp() {
					}
					const string display(const string& prefix) const;
				};

			}
		}
	}
}

#endif
