/*
 * ExpressionForCpp.h
 *
 *  Created on: 24 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef EXPRESSIONFORCPP_H_
#define EXPRESSIONFORCPP_H_

#include "model/Expression.h"

namespace Poco {
namespace FSM {
namespace PARSER {
namespace CPP {

class ExpressionForCpp : public virtual Poco::FSM::MODEL::Expression {
public:
	ExpressionForCpp() {
	}
	virtual ~ExpressionForCpp() {
	}
	virtual const string display(const string& prefix) const = 0;
};

}
}
}
}

#endif
