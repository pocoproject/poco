#ifndef _H_STATEMAP
#define _H_STATEMAP

//
// The contents of this file are subject to the Mozilla Public
// License Version 1.1 (the "License"); you may not use this file
// except in compliance with the License. You may obtain a copy
// of the License at http://www.mozilla.org/MPL/
//
// Software distributed under the License is distributed on an
// "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
// implied. See the License for the specific language governing
// rights and limitations under the License.
//
// The Original Code is State Machine Compiler (SMC).
//
// The Initial Developer of the Original Code is Charles W. Rapp.
// Portions created by Charles W. Rapp are
// Copyright (C) 2000 - 2007. Charles W. Rapp.
// All Rights Reserved.
//
// Contributor(s):
//
// Namespace
//	statemap
//
// Description
//  This namespace contains the finite state machine context
//  class. The user can derive FSM contexts from this class and
//  interface to them with the methods of this class.
//
// Notes
//  The finite state machine needs to be initialized to the
//  starting state of the FSM.  This must be done manually in
//  the constructor of the derived class.
//
// Author
//	C. W. Rapp
//
// RCS ID
// Id: statemap.h,v 1.19 2014/09/06 19:31:28 fperrad Exp
//
// CHANGE LOG
// (See bottom of file)
//
#if (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#if defined(SMC_USES_IOSTREAMS)
#include <iostream>
#endif // SMC_USES_IOSTREAMS
#if defined(SMC_NO_EXCEPTIONS)
#include <cassert>
#endif // SMC_NO_EXCEPTIONS
#include <cstdio>
#elif defined(WIN32)
#if defined(SMC_NO_EXCEPTIONS)
#include <cassert>
#endif // SMC_NO_EXCEPTIONS
#else
#if defined(SMC_NO_EXCEPTIONS)
#include <assert.h>
#endif // SMC_NO_EXCEPTIONS
#include <stdio.h>
#endif
#if ! defined(SMC_NO_EXCEPTIONS)
#include <stdexcept>
#include <cstring>
#endif

#define MAX_NAME_LEN 32

namespace statemap {
//---------------------------------------------------------------
// Routines.
//

#ifdef SMC_FIXED_STACK
// When static memory is used, a string has only one copy.
inline char* copyString(const char *s) {
	// Cast your const upon the waters and see what blows up.
	return (const_cast<char *>(s));
}
#else
inline char* copyString(const char *s) {
	char *retval = nullptr;

	if (s != nullptr) {
		retval = new char[MAX_NAME_LEN + 1];
		retval[MAX_NAME_LEN] = '\0';
		(void) std::strncpy(retval, s, MAX_NAME_LEN);
	}

	return (retval);
}
#endif
//---------------------------------------------------------------
// Exception Classes.
//

//#define SMC_NO_EXCEPTIONS
#ifndef SMC_NO_EXCEPTIONS
// Base class for all SMC exceptions.
class SmcException : public std::runtime_error {
  public:

	virtual ~SmcException() throw() {
	};

  protected:
	SmcException(const std::string& reason)
		: std::runtime_error(reason) {
	};

  private:
	SmcException();
};

#ifdef SMC_FIXED_STACK
class PushOnFullStateStackException :
	public SmcException {
	//-----------------------------------------------------------
	// Member methods.
	//
  public:

	// Default constructor.
	PushOnFullStateStackException()
		: SmcException("cannot push on full state stack") {
	};

	// Destructor.
	virtual ~PushOnFullStateStackException() throw() {
	};

  protected:
  private:

	//-----------------------------------------------------------
	// Member data.
	//
  public:
  protected:
  private:
};
#endif

// This class is thrown when a pop is issued on an empty
// state stack.
class PopOnEmptyStateStackException :
	public SmcException {
  public:

	// Default constructor.
	PopOnEmptyStateStackException()
		: SmcException("no state to pop from state stack") {
	};

	// Destructor.
	virtual ~PopOnEmptyStateStackException() throw() {
	};

};

// This class is thrown when a transition is issued
// but there is no current state. This happens when
// a transition is issued from within a transition
// action.
class StateUndefinedException :
	public SmcException {
  public:

	// Default constructor.
	StateUndefinedException()
		: SmcException("transition invoked while in transition") {
	};

	// Destructor.
	virtual ~StateUndefinedException() throw() {
	};

};

// This class is thrown when a transition is issued
// but there is no code to handle it.
class TransitionUndefinedException :
	public SmcException {
  public:

	// Default constructor.
	TransitionUndefinedException()
		: SmcException("no such transition in current state"),
		  _state(nullptr),
		  _transition(nullptr) {
	};

	// Construct an exception using the specified state
	// and transition.
	TransitionUndefinedException(const char *state,
								 const char *transition)
		: SmcException("no such transition in current state"),
		  _state(copyString(state)),
		  _transition(copyString(transition)) {
	};

	// Copy constructor.
	TransitionUndefinedException(
		const TransitionUndefinedException& ex)
		: SmcException("no such transition in current state"),
		  _state(copyString(ex._state)),
		  _transition(copyString(ex._transition)) {
	};

	virtual ~TransitionUndefinedException() throw() {
		if (_state != nullptr) {
			delete[] _state;
			_state = nullptr;
		}

		if (_transition != nullptr) {
			delete[] _transition;
			_transition = nullptr;
		}
	};

	const TransitionUndefinedException&
	operator=(const TransitionUndefinedException& ex) {
		// Don't do self assignment.
		if (this != &ex) {
			if (_state != nullptr) {
				delete[] _state;
				_state = nullptr;
			}

			if (_transition != nullptr) {
				delete[] _transition;
				_transition = nullptr;
			}

			_state = copyString(ex._state);
			_transition = copyString(ex._transition);
		}

		return (*this);
	};

	// Returns the state. May be nullptr.
	const char* getState() const {
		return(_state);
	};

	// Returns the transition. May be nullptr.
	const char* getTransition() const {
		return (_transition);
	};

  private:
	char *_state;
	char *_transition;
};

// This class is thrown when a state ID is either less than
// the minimal value or greater than the maximal value.
class IndexOutOfBoundsException :
	public SmcException {
  public:

	IndexOutOfBoundsException()
		: SmcException("index out of bounds"),
		  _index(0),
		  _minIndex(0),
		  _maxIndex(0) {
	};

	// Constructs an exception using the specified index,
	// minimum index and maximum index.
	IndexOutOfBoundsException(const int index,
							  const int minIndex,
							  const int maxIndex)
		: SmcException("index out of bounds"),
		  _index(index),
		  _minIndex(minIndex),
		  _maxIndex(maxIndex) {
	};

	// Copy constructor.
	IndexOutOfBoundsException(
		const IndexOutOfBoundsException& ex)
		: SmcException("index out of bounds"),
		  _index(ex._index),
		  _minIndex(ex._minIndex),
		  _maxIndex(ex._maxIndex) {
	};

	virtual ~IndexOutOfBoundsException() throw() {
	};

	const IndexOutOfBoundsException&
	operator=(const IndexOutOfBoundsException& ex) {
		// Don't do self assignment.
		if (this != &ex) {
			_index = ex._index;
			_minIndex = ex._minIndex;
			_maxIndex = ex._maxIndex;
		}

		return (*this);
	};

	// Returns the out-of-bounds index.
	int getIndex() const {
		return(_index);
	};

	// Returns the minimum allowed index value.
	int getMinIndex() const {
		return (_minIndex);
	};

	// Returns the maximum allowed index value.
	int getMaxIndex() const {
		return (_maxIndex);
	};

  private:
	int _index;
	int _minIndex;
	int _maxIndex;
};
#endif
//
// end of Exception Classes.
//---------------------------------------------------------------

class State {
public:
	const char* getName() const {
		return (_name);
	};

	int getId() const {
		return (_stateId);
	}

protected:
	  State() = delete;
	  State(const State&) = delete;

	State(const char *name, int stateId)
		: _name(nullptr),
		  _stateId(stateId) {
		if (name != nullptr) {
			_name = copyString(name);
		} else {
			_name = copyString("NAME NOT SET");
		}
	};

	virtual ~State() {
#ifndef SMC_FIXED_STACK
		if (_name != nullptr) {
			delete[] _name;
			_name = nullptr;
		}
#endif
	};

  private:
  public:
  protected:
	char *_name;	// This state's printable name.
	int _stateId;	// This state's unique identifier.

  private:
};

enum class Event {
	Enter,
	Leave,
	Switch,
	Transition,
	Push,
	Pop
};

class Notifier {
  public:
	virtual  void operator()(const Event& event, const char* s) = 0;
	virtual  void operator()(const Event& event, const State* state) = 0;
	virtual  void operator()(const Event& event, const State* previous, const State* next) = 0;
};
class FSMContext {
  private:
	class StateEntry {
	  public:
		StateEntry(State *state, StateEntry *next)
			: _state(state),
			  _next(next) {
		};

		~StateEntry() {
			_state = nullptr;
			_next = nullptr;
		};

		State* getState() {
			return(_state);
		};

		const State* getState() const {
			return(_state);
		};

		StateEntry* getNext() {
			return(_next);
		};

	  private:
		State *_state;
		StateEntry *_next;

		friend class FSMContext;
	};

  public:

	virtual ~FSMContext() {
#ifdef SMC_FIXED_STACK
		_transition = nullptr;
#else // ! SMC_FIXED_STACK
		StateEntry *state;

		if (_transition != nullptr) {
			delete[] _transition;
			_transition = nullptr;
		}

		// But we did allocate the state stack.
		while (_state_stack != nullptr) {
			state = _state_stack;
			_state_stack = _state_stack->_next;
			delete state;
		}
#endif // ! SMC_FIXED_STACK
	};

	FSMContext& operator=(const FSMContext& fsm) {
		// Don't do the assignment if the left and right
		// hand sides are the same object.
		if (this != &fsm) {
			_state = fsm._state;
		}

		return(*this);
	};

	// Starts the finite state machine running by executing
	// the initial state's entry actions.
	virtual void enterStartState()=0;

	// Exact same object (is it me?)
	int same(const FSMContext& fsm) const {
		return(this == &fsm);
	};

	bool getDebugFlag() {
		return(_debug_flag);
	};

	void setDebugFlag(bool flag) {
		_debug_flag = flag;
	};

	// Is this state machine already inside a transition?
	// Yes if state is null.
	bool isInTransition() const {
		return(_state == nullptr ? true : false);
	};

	// Returns the current transition's name.
	// Used only for debugging purposes.
	char* getTransition() const {
		return (_transition);
	};

	// Saves away the transition name only if debugging
	// is turned on.
	void setTransition(const char *transition) {
#ifndef SMC_FIXED_STACK
		if (_transition != nullptr) {
			delete[] _transition;
			_transition = nullptr;
		}
#endif // ! SMC_FIXED_STACK

		_transition = copyString(transition);
		if (getDebugFlag() && transition)
			_notify(Event::Transition, transition);

		return;
	};

	// Clears the current state.
	void clearState() {
		_previous_state = _state;
		_state = nullptr;
	};

	// Returns the state which a transition left.
	// May be nullptr.
	State* getPreviousState() {
		return (_previous_state);
	}

	// Sets the current state to the specified state.
	void setState(const State& state) {
		// clearState() is not called when a transition has
		// no actions, so set _previous_state to _state in
		// that situation. We know clearState() was not
		// called when _state is not null.
		if (_state != nullptr) {
			if (_debug_flag) {
				_notify(Event::Leave, _state);
			}
			_previous_state = _state;
		}

		_state = const_cast<State *>(&state);

		if (_debug_flag) {
			_notify(Event::Switch, _previous_state, _state);
		}
	};

#ifdef SMC_FIXED_STACK
	// Returns true if the state stack is empty and false
	// otherwise.
	bool isStateStackEmpty() const {
		return (_state_stack_depth == 0);
	}

	// Returns the state stack's depth.
	int getStateStackDepth() const {
		return (_state_stack_depth);
	}

	// Push the current state on top of the state stack
	// and make the specified state the current state.
	void pushState(const State& state) {
#ifdef SMC_NO_EXCEPTIONS
		assert (_state_stack_depth < SMC_STATE_STACK_SIZE);
#else
		if (_state_stack_depth == SMC_STATE_STACK_SIZE) {
			throw PushOnFullStateStackException();
		}
#endif

		// Do the push only if there is a state to be pushed
		// on the stack.
		if (_state != nullptr) {
			_state_stack[_state_stack_depth] = _state;
			++_state_stack_depth;
		}

		_previous_state = _state;
		_state = const_cast<State *>(&state);

		if (_debug_flag == true) {
			_notify(Event::PUSH_STATE, _state);
		}
	};

	// Make the state on top of the state stack the
	// current state.
	void popState() {
		// Popping when there was no previous push is an error.
#ifdef SMC_NO_EXCEPTIONS
		poco_assert(_state_stack_depth > 0);
#else
		if (_state_stack_depth == 0) {
			throw PopOnEmptyStateStackException();
		}
#endif

		_previous_state = _state;
		--_state_stack_depth;
		_state = _state_stack[_state_stack_depth];

		if (_debug_flag == true) {
			_notify(Event::POP, _state);
		}
	};

	// Remove all states from the state stack.
	void emptyStateStack() {
		_state_stack_depth = 0;
	};
#else // ! SMC_FIXED_STACK
	// Returns true if the state stack is empty and false
	// otherwise.
	bool isStateStackEmpty() const {
		return (_state_stack == nullptr);
	}

	// Returns the state stack's depth.
	int getStateStackDepth() const {
		StateEntry *state_ptr;
		int retval;

		for (state_ptr = _state_stack, retval = 0;
				state_ptr != nullptr;
				state_ptr = state_ptr->getNext(), ++retval)
			;

		return (retval);
	}

	// Push the current state on top of the state stack
	// and make the specified state the current state.
	void pushState(const State& state) {
		StateEntry *new_entry;

		// Do the push only if there is a state to be pushed
		// on the stack.
		if (_state != nullptr) {
			new_entry = new StateEntry(_state, _state_stack);
			_state_stack = new_entry;
		}

		_previous_state = _state;
		_state = const_cast<State *>(&state);

		if (_debug_flag == true) {
			_notify(Event::Push, _state);
		}
	};

	// Make the state on top of the state stack the
	// current state.
	void popState() {
		StateEntry *entry;

		// Popping when there was no previous push is an error.
#ifdef SMC_NO_EXCEPTIONS
		poco_assert(_state_stack != nullptr);
#else
		if (_state_stack == nullptr) {
			throw PopOnEmptyStateStackException();
		}
#endif // SMC_NO_EXCEPTIONS

		_previous_state = _state;
		_state = _state_stack->getState();
		entry = _state_stack;
		_state_stack = _state_stack->getNext();
		delete entry;

		if (_debug_flag == true) {
			_notify(Event::Pop, _state);
		}
	};

	// Remove all states from the state stack.
	void emptyStateStack() {
		StateEntry *state_ptr,  *next_ptr;

		for (state_ptr = _state_stack; state_ptr != nullptr; state_ptr = next_ptr) {
			next_ptr = state_ptr->getNext();
			delete state_ptr;
		}
		_state_stack = nullptr;
	};
#endif

  protected:
	  FSMContext(const FSMContext&) = delete;
	// Default constructor.
	FSMContext(Notifier& notifier, const State& state)
		: _state(const_cast<State *>(&state)),
		  _previous_state(nullptr),
#ifdef SMC_FIXED_STACK
		  _state_stack_depth(0),
#else
		  _state_stack(nullptr),
#endif
		  _transition(nullptr),
		  _debug_flag(false),
		  _notify(notifier)
	{};

  public:
  protected:

	// The current state of the finite state machine.
	State *_state;

	// Remember which state a transition left.
	State *_previous_state;

	// The stack of pushed states.
#ifdef SMC_FIXED_STACK
	State* _state_stack[SMC_STATE_STACK_SIZE];
	int _state_stack_depth;
#else
	StateEntry *_state_stack;
#endif
	char *_transition;

  private:
	bool _debug_flag;
	Notifier&	_notify;
};
}
#endif
