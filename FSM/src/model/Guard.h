/*
 * Guard.h
 *
 *  Created on: 16 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_MODEL_GUARD_H_
#define POCO_FSM_MODEL_GUARD_H_

#include "SMC.h"
#include "Element.h"

namespace Poco {
	namespace FSM {
		namespace MODEL {
			enum TransType
			{
				/**
				 * Transitions may be instantiated before all information
				 * is gathered. This is the default transition type.
				 */
				TRANS_NOT_SET,

				/**
				 * A standard transition which goes from one state to the
				 * next.
				 */
				 TRANS_SET,

				 /**
				  * This transition pushes the current state on top of the
				  * state stack before setting the next state. The previous
				  * state may be restored using a pop transition. A push
				  * transtion may be used in conjunction with a set
				  * transition. In that case the specified next state is
				  * pushed on top of the state stack and not the current
				  * state.
				  */
				  TRANS_PUSH,

				  /**
				   * Pop the state off the state stack and make it the next
				   * state.
				   */
				   TRANS_POP
			}; // end of enum TransType

			class Guard : public Element {
			private:
				TransitionPtr	_transition = nullptr;
				ExpressionPtr	_condition = nullptr;
				TransType		_transType;
				StatePtr		_endstate = nullptr;
				StatePtr		_pushState = nullptr;
				StatePtr		_popArgs = nullptr;
				List<ActionPtr>	_actions;

			public:
				Guard(const string& name, int lineno = 0) : Element(name, lineno) {
				}
				virtual ~Guard() {
				}

				TransitionPtr&					transition()	{ return _transition; }
				ExpressionPtr&					condition()		{ return _condition;  }
				StatePtr&						endstate()		{ return _endstate; }
				StatePtr&						pushState()		{ return _pushState; }
				StatePtr&						popArgs()		{ return _popArgs; }
				List<ActionPtr>&				actions()		{ return _actions; }

				const TransitionPtr&			transition()const 	{ return _transition; }
				const ExpressionPtr&			condition()const 	{ return _condition; }
				const StatePtr&					endstate()const 	{ return _endstate; }
				const StatePtr&					pushState()const 	{ return _pushState; }
				const StatePtr&					popArgs()const 		{ return _popArgs; }
				const List<ActionPtr>&			actions()	const 	{ return _actions; }

				void add(ActionPtr action)		{ _actions.push_back(action); }

				virtual const string display() const;
			};

		}
	}
}
#endif
