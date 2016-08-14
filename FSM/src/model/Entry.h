//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#ifndef POCO_FSM_MODEL_ENTRY_H_
#define POCO_FSM_MODEL_ENTRY_H_

#include "Element.h"
#include "Actions.h"

namespace Poco {
	namespace FSM {
		namespace MODEL {

			class Entry : public Actions, public Element {
			public:
				Entry(const string& name, int lineno = 0) : Element(name, lineno) {
				}
				virtual ~Entry() {
				}
				virtual const string display() const;
				void print(Print& print) const;
			};
		}
	}
}
#endif
