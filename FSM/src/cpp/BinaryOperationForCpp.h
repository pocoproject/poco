/*
 * BinaryOperationForCpp.h
 *
 *  Created on: 24 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef BINARYOPERATIONFORCPP_H_
#define BINARYOPERATIONFORCPP_H_

#include "model/BinaryOperation.h"
#include "ExpressionForCpp.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {
			namespace CPP {

				class BinaryOperationForCpp : public ExpressionForCpp, public FSM::MODEL::BinaryOperation {

				public:
					BinaryOperationForCpp(Poco::FSM::MODEL::Operator op) :
						FSM::MODEL::BinaryOperation(op) {
					}
					virtual ~BinaryOperationForCpp() {
					}
					const string display(const string& prefix) const;
				};
			}
		}
	}
}

#endif
