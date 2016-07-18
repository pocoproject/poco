/*
 * MapForCpp.h
 *
 *  Created on: 19 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef  POCO_FSM_CPP_MAP_H_
#define  POCO_FSM_CPP_MAP_H_

#include "model/Map.h"
#include "CompilableForCpp.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {
			namespace CPP {

				class MapForCpp : public CompilableForCpp, public FSM::MODEL::Map {
				public:
					MapForCpp(const string& name, int lineno = 0) :

						FSM::MODEL::Map(name, lineno) {
					}
					virtual ~MapForCpp() {
					}
					virtual void generateInclude(ostream& inc, bool debug) const;
					virtual void generateCode(ostream& cpp, bool debug) const;

					string defaultStateName() const;
					string startStateName() const;
				};

			}
		}
	}
}
#endif
