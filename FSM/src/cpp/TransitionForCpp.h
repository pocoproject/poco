/*
 * TransitionForCpp.h
 *
 *  Created on: 19 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef  POCO_FSM_CPP_TRANSITION_H_
#define  POCO_FSM_CPP_TRANSITION_H_

#include "model/Transition.h"
#include "CompilableForCpp.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {
			namespace CPP {

				class TransitionForCpp : public CompilableForCpp, public FSM::MODEL::Transition {
				public:
					TransitionForCpp(const string& name, int lineno = 0) :
						FSM::MODEL::Transition(name, lineno) {
					}
					virtual ~TransitionForCpp() {
					}
					virtual const string name() const;
					virtual void generateInclude(ostream& inc, bool debug) const;
					virtual void generateCode(ostream& cpp, bool debug) const;
					void generateVirtualTransitions(ostream& inc, bool debug) const;
					void generateDefinition(ostream& inc, bool debug) const;
				};

			}
		}
	}
}
#endif
