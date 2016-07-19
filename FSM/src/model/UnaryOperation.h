/*
 * UnaryOperation.h
 *
 *  Created on: 20 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_MODEL_UNARY_OPERATION_H_
#define POCO_FSM_MODEL_UNARY_OPERATION_H_

#include "Operation.h"
#include "Expression.h"

namespace Poco {
	namespace FSM {
		namespace MODEL {

			class UnaryOperation : public Operation {
			private:
				ExpressionPtr		_operand;

			public:
				UnaryOperation(Operator op) : Operation(op) {
				}
				virtual ~UnaryOperation() {
				}

				ExpressionPtr& operand()				{ return _operand; }
				const ExpressionPtr  operand() const	{ return _operand; }
				virtual const string display() const;
			};

		}
	}
}
#endif
