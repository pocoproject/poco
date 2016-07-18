/*
 * Expression.h
 *
 *  Created on: 21 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_MODEL_EXPRESSION_H_
#define POCO_FSM_MODEL_EXPRESSION_H_

#include "SMC.h"

namespace Poco {
	namespace FSM {
		namespace MODEL {

			class Expression {
			protected:
				OperationPtr	_operation = NULL;
				mutable string	_asstring;
				GuardPtr		_guard = NULL;

			public:
				Expression() {
				}
				virtual ~Expression() {
				}
				GuardPtr&		guard()				{ return _guard; }
				const GuardPtr	guard() const 		{ return _guard; }

				virtual const string display() const = 0;
				OperationPtr&		operation()			{ return _operation;  }
				const OperationPtr&	operation() const	{ return _operation;  }
			};

		}
	}
}
#endif
