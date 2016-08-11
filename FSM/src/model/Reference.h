/*
 * Reference.h
 *
 *  Created on: 1 févr. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_MODEL_REFERENCE_H_
#define POCO_FSM_MODEL_REFERENCE_H_

#include "SMC.h"
#include "Expression.h"
#include "Element.h"
#include "Variable.h"
#include "Literal.h"
#include "Function.h"
#include "Operation.h"

namespace Poco {
namespace FSM {
namespace MODEL {

class Reference  : public virtual Expression, public Element {
private:

	union {
		VariablePtr	_variable;
		LiteralPtr  _literal;
		FunctionPtr  _function;
	};

public:
	Reference(const VariablePtr variable, int lineno = 0) : 
		Element(lineno), 
		_variable(variable)  {
	}
	Reference(const FunctionPtr function, int lineno = 0) :
		Element(lineno),
		_function(function)  {
	}
	Reference(const LiteralPtr literal, int lineno = 0) :
		Element(lineno),
		_literal(literal)  {
	}
	virtual ~Reference() {
	}
	virtual const string display() const;
	ElementPtr			reference()			{ return _variable;  }
	const ElementPtr	reference() const	{ return _variable;  }
	bool isParameter() const;
};

}
}
}

#endif
