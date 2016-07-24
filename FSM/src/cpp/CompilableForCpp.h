/*
 * CompilableForCpp.h
 *
 *  Created on: 24 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef COMPILABLEFORCPP_H_
#define COMPILABLEFORCPP_H_

#include <iostream>
using std::ostream;

#include "model/Compilable.h"
using Poco::FSM::MODEL::FSMPtr;

namespace Poco {
namespace FSM {
namespace PARSER {
namespace CPP {

class CompilableForCpp : public Poco::FSM::MODEL::Compilable {
public:
	CompilableForCpp() : Poco::FSM::MODEL::Compilable() {
	}
	virtual ~CompilableForCpp() {
	}
	virtual void generate(const Path& path, bool debug) const {}
	virtual void generateInclude(ostream& inc, bool debug) const = 0;
	virtual void generateCode(ostream& cpp, bool debug) const = 0;
};

}
}
}
}

#endif
