/*
 * Dumper.h
 *
 *  Created on: 8 févr. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_MODEL_DUMPER_H_
#define POCO_FSM_MODEL_DUMPER_H_

#include <string>
using std::string;

#include "FSM.h"
using Poco::FSM::MODEL::TransitionPtr;
using Poco::FSM::MODEL::GuardPtr;

namespace Poco {
namespace FSM {
namespace MODEL {

class Dumper {
public:
	virtual void operator()(const char* message) = 0;
	virtual void operator()(const string& message) = 0;
	virtual void operator()(const TransitionPtr transition, const GuardPtr guard) = 0;
};

}
}
}

#endif
