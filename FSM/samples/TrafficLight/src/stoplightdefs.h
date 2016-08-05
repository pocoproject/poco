#ifndef _H_STOPLIGHTDEFS
#define _H_STOPLIGHTDEFS

//
// The contents of this file are subject to the Mozilla Public
// License Version 1.1 (the "License"); you may not use this file
// except in compliance with the License. You may obtain a copy of
// the License at http://www.mozilla.org/MPL/
//
// Software distributed under the License is distributed on an "AS
// IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
// implied. See the License for the specific language governing
// rights and limitations under the License.
//
// The Original Code is State Machine Compiler (SMC).
//
// The Initial Developer of the Original Code is Charles W. Rapp.
// Portions created by Charles W. Rapp are
// Copyright (C) 2000 - 2003 Charles W. Rapp.
// All Rights Reserved.
//
// Contributor(s):
//
// Name
//	TheContext
//
// Description
//	When a state machine executes an action, it is really calling a
//	member function in the context class.
//

#ifdef WIN32
// Use this value when setting the stoplight timer.
#define STOPLIGHT_TIMER 1
#endif

namespace cpp_ex4 {
enum LightColors {
	GREEN = 0,
	YELLOW,
	RED
};

enum StopLights {
	EWLIGHT = 1,
	NSLIGHT
};

enum Directions {
	NORTH_SOUTH,
	EAST_WEST
};
}

#endif
