/*
 * Literal.h
 *
 *  Created on: 21 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_MODEL_LITERAL_H_
#define POCO_FSM_MODEL_LITERAL_H_

#include "SMC.h"
#include "Expression.h"
#include "Element.h"

namespace Poco {
	namespace FSM {
		namespace MODEL {

			class Literal : public Element {
			public:
				Literal(const string& name, int lineno = 0) : Element(name, lineno)  {
				}
				virtual ~Literal() {
				}
				virtual const string display() const;
				virtual const string display(const string& prefix) const;
			};

		}
	}
}
#endif
