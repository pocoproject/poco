/*
 * ExitForCpp.h
 *
 *  Created on: 26 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef EXITFORCPP_H_
#define EXITFORCPP_H_

#include "model/Exit.h"
#include "CompilableForCpp.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {
			namespace CPP {

				class ExitForCpp : public CompilableForCpp, public FSM::MODEL::Exit {
				public:
					ExitForCpp(const string& name, int lineno = 0) :
						FSM::MODEL::Exit(name, lineno) {
					}
					virtual ~ExitForCpp() {
					}
					virtual void generateInclude(ostream& inc, bool debug) const;
					virtual void generateCode(ostream& cpp, bool debug) const;
				};

			}
		}
	}
}

#endif
