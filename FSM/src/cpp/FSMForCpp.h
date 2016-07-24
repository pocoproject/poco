/*
 * FSMForCpp.h
 *
 *  Created on: 19 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef  POCO_FSM_CPP_FSM_H_
#define  POCO_FSM_CPP_FSM_H_


#include "model/FSM.h"
#include "CompilableForCpp.h"
#include "parser/IndentStream.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {
			namespace CPP {

				class FSMForCpp : public Poco::FSM::MODEL::FSM {
				public:
					FSMForCpp(Parser* parser) :
						Poco::FSM::MODEL::FSM(parser) {
					}
					virtual ~FSMForCpp() {
					}
					void generate(const Path& out, bool debug) const;
					virtual bool updated(const Path& out);

				private:
					void openNamespaces(IndentStream& os) const;
					void closeNamespaces(IndentStream& os) const;
				};

			}
		}
	}
}
#endif
