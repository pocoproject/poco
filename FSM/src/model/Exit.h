/*
 * Exit.h
 *
 *  Created on: 26 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_MODEL_EXIT_H_
#define POCO_FSM_MODEL_EXIT_H_

#include "Print.h"
#include "Element.h"
#include "Actions.h"

namespace Poco {
namespace FSM {
namespace MODEL {

			class Exit : public Actions, public Element {
			public:
				Exit(const string& name, int lineno = 0) : Element(name, lineno) {
				}
				virtual ~Exit() {
				}
				virtual const string display() const;
				void print(Print& print) const;
			};

}
}
}

#endif
