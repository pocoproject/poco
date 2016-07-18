/*
 * Print.h
 *
 *  Created on: 22 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_MODEL_PRINT_H_
#define POCO_FSM_MODEL_PRINT_H_

namespace Poco {
	class Logger;
}
using Poco::Logger;

#include "Dumper.h"

namespace Poco {
namespace FSM {
namespace MODEL {

class Print : public Dumper {
public:
	Print(Logger& logger) : logger(logger) {
	}
	virtual ~Print() {
	}
	virtual void operator()(const char* message);
	virtual void operator()(const string& message);
	virtual void operator()(const TransitionPtr transition, const GuardPtr guard);

private:
	Logger& logger;

};

}
}
}

#endif
