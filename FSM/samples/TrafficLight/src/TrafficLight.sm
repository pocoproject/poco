// -*- tab-width: 4; -*-

%{
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
// State Machine
//	Stoplight
//
// Description
//  This state machine implements a US traffic light.
//
%}
%start StopMap::NorthSouthGreen
%class Stoplight
%fsmclass StoplightContext
%include "stoplightdefs.h"
%header stoplight.h
%package cpp_ex4

%map StopMap
%%
EastWestGreen
	Entry	{ TurnLight(NSLIGHT, RED); TurnLight(EWLIGHT, GREEN);  }
{
	Timeout		EastWestYellow		{ setTimer(Stoplight::YellowTimer); }
}

EastWestYellow
	Entry	{ TurnLight(EWLIGHT, YELLOW); }
{
	Timeout		NorthSouthGreen		{ setTimer(Stoplight::NSGreenTimer); }
}

NorthSouthGreen
	Entry	{ TurnLight(EWLIGHT, RED); TurnLight(NSLIGHT, GREEN);  }
{
	Timeout		NorthSouthYellow	{ setTimer(Stoplight::YellowTimer); }
}

NorthSouthYellow
	Entry	{ TurnLight(NSLIGHT, YELLOW); }
{
	Timeout		EastWestGreen		{ setTimer(Stoplight::EWGreenTimer); }
}
%%
