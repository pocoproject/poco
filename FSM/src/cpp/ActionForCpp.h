/*
 * ActionForCpp.h
 *
 *  Created on: 19 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef  POCO_FSM_CPP_ACTION_H_
#define  POCO_FSM_CPP_ACTION_H_

#include "model/Action.h"
#include "CompilableForCpp.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {
			namespace CPP {

				class ActionForCpp : public CompilableForCpp, public FSM::MODEL::Action {
				public:
					ActionForCpp(const string& name, int lineno = 0) :
						FSM::MODEL::Action(name, lineno) {
					}
					virtual ~ActionForCpp() {
					}
					virtual const string name() const;
					virtual void generateInclude(ostream& inc, bool debug) const;
					virtual void generateCode(ostream& cpp, bool debug) const;
				};

			}
		}
	}
}
#endif
