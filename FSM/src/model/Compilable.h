/*
 * Compilable.h
 *
 *  Created on: 22 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_MODEL_COMPILABLE_H_
#define POCO_FSM_MODEL_COMPILABLE_H_

#include "Poco/Path.h"
using Poco::Path;


#include "SMC.h"

namespace Poco {
	namespace FSM {
		namespace MODEL {

			class Compilable {

			public:
				virtual void generate(const Path& out, bool debug) const = 0;
			};

		}
	}
}

#endif
