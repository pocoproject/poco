//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#ifndef POCO_FSM_MODEL_VARIABLE_H_
#define POCO_FSM_MODEL_VARIABLE_H_

#include "SMC.h"
#include "Expression.h"
#include "Element.h"

namespace Poco {
	namespace FSM {
		namespace MODEL {

			class Variable : public Element {
			public:
				Variable(const string& name, int lineno = 0) : Element(name, lineno)  {
				}
				virtual ~Variable() {
				}
				virtual const string display() const;
			};

		}
	}
}
#endif
