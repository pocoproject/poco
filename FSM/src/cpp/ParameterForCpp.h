/*
 * ParameterForCpp.h
 *
 *  Created on: 19 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef  POCO_FSM_CPP_PARAMETER_H_
#define  POCO_FSM_CPP_PARAMETER_H_

#include "model/Parameter.h"
#include "CompilableForCpp.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {
			namespace CPP {

				class ParameterForCpp : public CompilableForCpp, public FSM::MODEL::Parameter {
				public:
					ParameterForCpp(const string& name, int lineno = 0) :
						FSM::MODEL::Parameter(name, lineno) {
					}
					virtual ~ParameterForCpp() {
					}
					virtual void generateInclude(ostream& inc, bool debug) const;
					virtual void generateCode(ostream& cpp, bool debug) const;
				};

			}
		}
	}
}
#endif
