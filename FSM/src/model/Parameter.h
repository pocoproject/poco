/*
 * Parameter.h
 *
 *  Created on: 16 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_MODEL_PARAMETER_H_
#define POCO_FSM_MODEL_PARAMETER_H_


#include "SMC.h"
#include "Element.h"
#include "Reference.h"

namespace Poco {
	namespace FSM {
		namespace MODEL {

			class Parameter : public Element {
			private:
				string _type;

			public:
				Parameter(const string& name, int lineno = 0) : Element(name, lineno) {
				}
				virtual ~Parameter() {
				}

				string& type()					{ return _type; }
				const string& type() const		{ return _type; }
				virtual const string display() const;
			};

		}
	}
}
#endif
