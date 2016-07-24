#ifndef _H_INPUTLISTENER
#define _H_INPUTLISTENER

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
// Name
//  InputListener.h
//
// Description
//  Input event handler.
//
// RCS ID
// $Id: InputListener.h,v 1.5 2007/12/28 12:34:40 cwrapp Exp $
//
// CHANGE LOG
// $Log: InputListener.h,v $
// Revision 1.5  2007/12/28 12:34:40  cwrapp
// Version 5.0.1 check-in.
//
// Revision 1.4  2005/05/28 13:31:18  cwrapp
// Updated C++ examples.
//
// Revision 1.0  2003/12/14 19:38:51  charlesr
// Initial revision
//

class InputListener
{
// Member functions.
public:

    // Default constructor.
    InputListener() {};

    // Destructor.
    virtual ~InputListener() {};

    // Input callback method.
    virtual void handleReceive(int fd)=0;

protected:
private:

// Member data.
public:
protected:
private:
};

#endif
