//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//


#ifndef POCO_FSM_MODEL_ACTION_H_
#define POCO_FSM_MODEL_ACTION_H_

#include "SMC.h"
#include "Argument.h"
#include "Element.h"

namespace Poco {
	namespace FSM {
		namespace MODEL {

			class Action : public Element {
			private:
				List<ArgumentPtr> _arguments;

			public:
				Action(const string& name, int lineno = 0) : Element(name, lineno) {
				}
				virtual ~Action() {
				}
				List<ArgumentPtr>& arguments()				{ return _arguments; }
				const List<ArgumentPtr>& arguments() const	{ return _arguments; }

				void add(const ArgumentPtr argument)		{ _arguments.push_back(argument); }
				virtual const string display() const;
				void print(Print& print) const;
			};

		}
	}
}
#endif
