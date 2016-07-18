/*
 * BinaryOperation.h
 *
 *  Created on: 20 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_MODEL_BINARY_OPERATION_H_
#define POCO_FSM_MODEL_BINARY_OPERATION_H_

#include "Expression.h"
#include "Operation.h"

namespace Poco {
	namespace FSM {
		namespace MODEL {

			class BinaryOperation : public Operation {
			private:
				ExpressionPtr		_left;
				ExpressionPtr		_right;

			public:
				BinaryOperation(Operator op) : Operation(op) {
				}
				virtual ~BinaryOperation() {
				}
				ExpressionPtr&	left()				{ return _left; }
				ExpressionPtr&	right()				{ return _right; }
				const ExpressionPtr	left() const	{ return _left; }
				const ExpressionPtr	right()const	{ return _right; }
				virtual const string display() const;
			};

		}
	}
}

#endif

#if 0
conditional-expression:
    logical-or-expression
    logical-or-expression ? expression : conditional-expression
logical-or-expression:
    logical-and-expression
    logical-or-expression   ||   logical-and-expression
logical-and-expression:
    inclusive-or-expression
    logical-and-expression   &&   inclusive-or-expression
inclusive-or-expression:
    exclusive-or-expression
    inclusive-or-expression | exclusive-or-expression
exclusive-or-expression:
    and-expression
    exclusive-or-expression ^ and-expression
and-expression:
    equality-expression
    and-expression & equality-expression
equality-expression:
    relational-expression
    equality-expression == relational-expression
    equality-expression != relational-expression
relational-expression:
    shift-expression
    relational-expression < shift-expression
    relational-expression > shift-expression
    relational-expression <= shift-expression
    relational-expression => shift-expression
shift-expression:
    additive-expression
    shift-expression << additive-expression
    shift-expression >> additive-expression
additive-expression:
    multiplicative-expression
    additive-expression + multiplicative-expression
    additive-expression – multiplicative-expression
multiplicative-expression:
    segment-expression
    multiplicative-expression * segment-expression
    multiplicative-expression / segment-expression
    multiplicative-expression % segment-expression
segment-expression:
    pm-expression
    segment-expression :> pm-expression
pm-expression:
    cast-expression
    pm-expression .* cast-expression
    pm-expression –>* cast-expression
cast-expression:
    unary-expression
    ( type-name ) cast-expression 
unary-expression:
    postfix-expression
    ++ unary-expression
    –– unary-expression
    unary-operator cast-expression
    sizeof unary-expression
    sizeof ( type-name )
    allocation-expression
    deallocation-expression
unary-operator: one of
    * & + – ! ~
#endif
