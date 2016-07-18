/*
 * EntryForCpp.h
 *
 *  Created on: 26 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef ENTRYFORCPP_H_
#define ENTRYFORCPP_H_

#include "model/Entry.h"
#include "CompilableForCpp.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {
			namespace CPP {

				class EntryForCpp : public CompilableForCpp, public FSM::MODEL::Entry {
				public:
					EntryForCpp(const string& name, int lineno = 0) :
						FSM::MODEL::Entry(name, lineno) {
					}
					virtual ~EntryForCpp() {
					}
					virtual void generateInclude(ostream& inc, bool debug) const;
					virtual void generateCode(ostream& cpp, bool debug) const;
				};

			}
		}
	}
}

#endif
