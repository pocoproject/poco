/*
 * GuardForCpp.h
 *
 *  Created on: 19 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef  POCO_FSM_CPP_GUARD_H_
#define  POCO_FSM_CPP_GUARD_H_

#include "model/Guard.h"
#include "CompilableForCpp.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {
			namespace CPP {

				class GuardForCpp : public CompilableForCpp, public FSM::MODEL::Guard {
				public:
					GuardForCpp(const string& name, int lineno = 0) :

						FSM::MODEL::Guard(name, lineno) {
					}
					virtual ~GuardForCpp() {
					}
					virtual void generateInclude(ostream& inc, bool debug) const;
					virtual void generateCode(ostream& cpp, bool debug) const;
				};

			}
		}
	}
}

#endif
