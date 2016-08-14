//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#ifndef POCO_FSM_MODEL_ACTIONS_H_
#define POCO_FSM_MODEL_ACTIONS_H_

#include "Element.h"

namespace Poco {
	namespace FSM {
		namespace MODEL {

			class Actions {
			protected:
				List<ActionPtr>	_actions;
			public:
				Actions() {
				}
				virtual ~Actions() {
				}
				
				List<ActionPtr>&		actions()			{ return _actions; }
				const List<ActionPtr>&	actions() const		{ return _actions; }
				
				void add(ActionPtr action)	{ _actions.push_back(action); }
			};

		}
	}
}

#endif
