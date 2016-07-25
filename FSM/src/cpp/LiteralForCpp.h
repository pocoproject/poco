//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#ifndef LITERALFORCPP_H_
#define LITERALFORCPP_H_

#include "model/Literal.h"
#include "CompilableForCpp.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {
			namespace CPP {

				class LiteralForCpp : public FSM::MODEL::Literal {
				public:
					LiteralForCpp(const string& name, int lineno = 0) :
						FSM::MODEL::Literal(name, lineno) {
					}
					virtual ~LiteralForCpp() {
					}
				};

			}
		}
	}
}

#endif
