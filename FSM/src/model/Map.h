//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#ifndef POCO_FSM_MODEL_MAP_H_
#define POCO_FSM_MODEL_MAP_H_

#include<map>
using std::map;

#include "SMC.h"
#include "Element.h"
#include "State.h"
#include "Print.h"

namespace Poco {
	namespace FSM {
		namespace MODEL {

			class Map : public Element {
			public:
				Map(const string& name, int lineno = 0);
				virtual ~Map();

				StatePtr&		defaultState()			{ return _defaultState; }
				const StatePtr	defaultState() const	{ return _defaultState; }
				FSMPtr&			fsm()					{ return _fsm; }
				const FSMPtr	fsm() const				{ return _fsm; }

				const map<string, StatePtr>&			states() const		{ return _states; }
				const map<string, TransitionPtr>&		transitions() const	{ return _transitions; }


				void add(TransitionPtr transition);
				void add(StatePtr state);
				StatePtr lookfor(const string& state) const;
				virtual const string display() const;
				void print(Print& print) const;

			protected:
				FSMPtr _fsm;
				StatePtr _defaultState;
				map<string, StatePtr> _states;
				map<string, TransitionPtr> _transitions;

			};

		}
	}
}
#endif
