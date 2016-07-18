/*
 * Argument.h
 *
 *  Created on: 21 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_MODEL_ARGUMENT_H_
#define POCO_FSM_MODEL_ARGUMENT_H_

#include "Element.h"

namespace Poco {
	namespace FSM {
		namespace MODEL {

			class Argument : public Element {
			public:
				Argument(const string& name, int lineno = 0) : Element(name, lineno) {
				}
				virtual ~Argument() {
				}
				virtual const string display() const;
			};

		}
	}
}

#endif
