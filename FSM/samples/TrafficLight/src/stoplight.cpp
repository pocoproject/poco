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
// Class
//	Stoplight
//
// Member Functions
//	Stoplight()						   - Default constructor.
//	Stoplight(Directions)			   - Set initial direction.
//	TurnLight(StopLights, LightColors) - Change directions.
//	SetTimer(int)					   - Start a timer.
//	Initialize(Directions)			   - Set start state and timer.
//

#include <iostream>
#include "Poco/Thread.h"
#include "Poco/Stopwatch.h"
#include "stoplight.h"

static const char tab = '\t';

using namespace std;
using Poco::TimerCallback;
using Poco::Stopwatch;
using Poco::Thread;

namespace cpp_ex4 {

int Stoplight::YellowTimer;
int Stoplight::NSGreenTimer;
int Stoplight::EWGreenTimer;

const char* literal(const statemap::Event & event) {
	switch (event) {
		case statemap::Event::Enter:		return "Enter     ";
		case statemap::Event::Leave:		return "Leave     ";
		case statemap::Event::Switch:		return "Switch    ";
		case statemap::Event::Transition:	return "Transition";
		case statemap::Event::Push:			return "Push      ";
		case statemap::Event::Pop:			return "Pop       ";
	}
}
class Print : public statemap::Notifier {
public:
	void operator ()(const statemap::Event & event, const char * message) {
		clog << tab << tab << literal(event) << ": " << message << endl;
	}
	void operator ()(const statemap::Event & event, const statemap::State * state)  {
//		clog << tab << tab << literal(event) << ": " << state->getName() << endl;
	}
	void operator ()(const statemap::Event & event, const statemap::State * fromState, const statemap::State *toState)  {
		clog << tab << tab << literal(event) << ": " << fromState->getName() << " => " << toState->getName()  << endl << endl;
	}
};
static Print print;

 Stoplight::Stoplight(Directions direction, int NSGreenTimer, int EWGreenTimer)	:
	StoplightContext(print),
	callback(*this, &Stoplight::onTimer) {

#define FSM_DEBUG
#ifdef FSM_DEBUG
	setDebugFlag(true);
#endif
	switch(direction) {
	case NORTH_SOUTH:
		cout << "Turning the north-south lights green." << endl;
		setStartState(NorthSouthGreen);
		timer.setStartInterval(NSGreenTimer * 1000);
		break;

	case EAST_WEST:
		cout << "Turning the east-west lights green." << endl;
		setStartState(EastWestGreen);
		timer.setStartInterval(EWGreenTimer * 1000);
		break;
	}
	this->NSGreenTimer = NSGreenTimer;
	this->EWGreenTimer = EWGreenTimer;
}

void Stoplight::start(int YellowTimer) {
	this->YellowTimer = YellowTimer;
	enterStartState();
	timer.start(callback);

	Stopwatch sw;
	sw.start();
	_event.wait();
	sw.stop();
	return;
}

void Stoplight::TurnLight(StopLights light, LightColors color) {
	cout << "Turning the ";

	switch(light) {
	case EWLIGHT:
		cout << "east-west   lights ";
		break;

	case NSLIGHT:
		cout << "north-south lights ";
		break;
	}

	switch(color) {
	case GREEN:
		cout << "green." << endl;
		break;

	case YELLOW:
		cout << "yellow." << endl;
		break;

	case RED:
		cout << "red." << endl;
		break;
	}

	return;
}

void Stoplight::onTimer(Timer& t) {
//	clog << ">>> onTimer: " << endl;
	Timeout();
//	clog << "<<< onTimer: " << endl;
}

void Stoplight::setTimer(int seconds) {
//	clog << ">>> setTimer: " << seconds << endl;
	timer.setPeriodicInterval(seconds * 1000);
	timer.restart();
//	clog << "<<< setTimer: " << seconds << endl;
}

}
