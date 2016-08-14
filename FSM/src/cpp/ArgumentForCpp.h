//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#ifndef ARGUMENTFORCPP_H_
#define ARGUMENTFORCPP_H_

#include "model/Argument.h"
#include "CompilableForCpp.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {
			namespace CPP {
				class ArgumentForCpp : public CompilableForCpp, public FSM::MODEL::Argument {
				public:
					ArgumentForCpp(const string& name, int lineno = 0) :
						FSM::MODEL::Argument(name, lineno) {
					}
					virtual ~ArgumentForCpp() {
					}
					virtual void generateInclude(ostream& inc, bool debug) const;
					virtual void generateCode(ostream& cpp, bool debug) const;
				};

			}
		}
	}
}

#endif
