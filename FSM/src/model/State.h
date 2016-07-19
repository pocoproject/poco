/*
 * State.h
 *
 *  Created on: 16 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_MODEL_STATE_H_
#define POCO_FSM_MODEL_STATE_H_

#include <map>
#include <string>
using std::string;

#include "SMC.h"
#include "Element.h"

using Poco::FSM::MODEL::TransitionPtr;

namespace Poco {
	namespace FSM {
		namespace MODEL {

			class State : public Element {
			public:
				State(const string& name, int lineno = 0) : Element(name, lineno) {
				}
				virtual ~State() {
				}
				MapPtr&			map()								{ return _map; }
				EntryPtr&		entry()								{ return _entry; }
				ExitPtr&		exit()								{ return _exit; }
				const MapPtr&	map()	const 						{ return _map; }
				const EntryPtr& entry()	const 						{ return _entry; }
				const ExitPtr&	exit()	const 						{ return _exit; }
				const List<TransitionPtr>&	transitions() const		{ return _transitions; }

				TransitionPtr add(TransitionPtr transition);
				TransitionPtr get(const string& transition, const List<ParameterPtr>& parameters) const;

				virtual const string display() const;
				void print(Print& print) const;

			protected:
				MapPtr				_map = nullptr;
				string				_className;
				string				_instanceName;
				EntryPtr			_entry = nullptr;
				ExitPtr				_exit = nullptr;
				List<TransitionPtr>	_transitions;

			private:
				std::map<string, TransitionPtr>	maps;

			};

		}
	}
}
#endif
