//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

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
