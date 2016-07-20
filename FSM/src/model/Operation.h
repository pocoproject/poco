/*
 * Operation.h
 *
 *  Created on: 20 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_MODEL_OPERATION_H_
#define POCO_FSM_MODEL_OPERATION_H_

#include "Expression.h"

namespace Poco {
	namespace FSM {
		namespace MODEL {
			enum Operator {
				none,
				and,
				or,
				xor,
				not,
				equal,
				notequal
			};
			class Operation : public virtual Expression {
			protected:
				Operation() : _contextuel(true) {}

				bool _contextuel;
				Operator		_operator;

			public:
				Operation(Operator op);
				virtual ~Operation();

				const bool&	contextuel() const		{ return _contextuel; }
				const Operator&		op() const		{ return _operator; }

				void setNoContextuel();
			};

		}
	}
}
#endif
