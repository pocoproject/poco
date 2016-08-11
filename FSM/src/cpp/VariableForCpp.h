/*
 * VariableForCpp.h
 *
 *  Created on: 21 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef VARIABLEFORCPP_H_
#define VARIABLEFORCPP_H_

#include "model/Variable.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {
			namespace CPP {

				class VariableForCpp : public FSM::MODEL::Variable {
				public:
					VariableForCpp(const string& name, int lineno = 0) :
						FSM::MODEL::Variable(name, lineno) {
					}
					virtual ~VariableForCpp() {
					}
					const string display(const string& prefix) const;
				};

			}
		}
	}
}

#endif
