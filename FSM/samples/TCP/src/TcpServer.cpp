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
//  TcpServer.cpp
//
// Description
//  TCP server connection class implementation.
//
// RCS ID
// $Id: TcpServer.cpp,v 1.5 2007/12/28 12:34:40 cwrapp Exp $
//
// CHANGE LOG
// $Log: TcpServer.cpp,v $
// Revision 1.5  2007/12/28 12:34:40  cwrapp
// Version 5.0.1 check-in.
//
// Revision 1.4  2005/05/28 13:31:18  cwrapp
// Updated C++ examples.
//
// Revision 1.0  2003/12/14 19:43:00  charlesr
// Initial revision
//

#include "TcpServer.h"

//---------------------------------------------------------------
// TcpServer(TcpConnectionListener&) (Public)
// Create a TCP service object.
//
TcpServer::TcpServer(TcpConnectionListener& listener)
: TcpConnection(listener)
{}

//---------------------------------------------------------------
// ~TcpServer() (Public)
// Destructor.
//
TcpServer::~TcpServer()
{}

//---------------------------------------------------------------
// getPort() const (Public)
// Return this service's TCP port.
//
unsigned short TcpServer::getPort() const
{
    return(_nearAddress.sin_port);
} // end of TcpServer::getPort() const

//---------------------------------------------------------------
// getAddress() const (Public)
// Return this service's IP address.
//
unsigned long TcpServer::getAddress() const
{
    return(_nearAddress.sin_addr.s_addr);
} // end of TcpServer::getAddress() const

//---------------------------------------------------------------
// open(unsigned short) (Public)
// Open this TCP service.
//
void TcpServer::open(unsigned short port)
{
    passiveOpen(port);
    return;
} // end of TcpServer::open(unsigned short)
