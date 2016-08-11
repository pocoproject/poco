#ifndef _H_STOPLIGHT
#define _H_STOPLIGHT

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
// Copyright (C) 2000 - 2009. Charles W. Rapp.
// All Rights Reserved.
//
// Contributor(s):
//
// Name
//	TheContext
//
// Description
//	When a state map executes an action, it is really calling a
//	member function in the context class.
//

#include "Poco/Timer.h"
#include "Poco/Event.h"
using Poco::Timer;
using Poco::Event;
using Poco::TimerCallback;

#include "stoplightdefs.h"
#include "stoplight_sm.h"

namespace cpp_ex4 {
	class Stoplight : protected StoplightContext<Stoplight> {
		// Member data.
	public:
		static int YellowTimer;
		static int NSGreenTimer;
		static int EWGreenTimer;

protected:
		void onTimer(Timer& t);

private:
		Event _event;
		Timer timer;
		TimerCallback<Stoplight> callback;

		// Member functions.
public:
		// Specify the initial direction with the green light.
		Stoplight(Directions direction, int NSGreenTimer, int EWGreenTimer);

		// Destructor.
		virtual ~Stoplight() {};

		void start(int YellowTimer);

		// Change a stoplight's color.
		void TurnLight(StopLights light, LightColors color);

		// Set a timer for the specified number of seconds.
		void setTimer(int time);


		// Sets the initial state of the state map and the initial timer.
		//static Stoplight* Initialize(Directions direction);


	};
}
#endif
