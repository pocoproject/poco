#ifndef _H_TCPCLIENT
#define _H_TCPCLIENT

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
//  TcpClient.h
//
// Description
//  TCP client connection class header.
//
// RCS ID
// $Id: TcpClient.h,v 1.5 2007/12/28 12:34:40 cwrapp Exp $
//
// CHANGE LOG
// $Log: TcpClient.h,v $
// Revision 1.5  2007/12/28 12:34:40  cwrapp
// Version 5.0.1 check-in.
//
// Revision 1.4  2005/05/28 13:31:18  cwrapp
// Updated C++ examples.
//
// Revision 1.0  2003/12/14 19:40:04  charlesr
// Initial revision
//

#include "TcpConnection.h"

// Forward declarations.
class TcpServer;

class TcpClient: public TcpConnection
{
// Member functions.
public:

    // Create an unconnected client.
    TcpClient(TcpConnectionListener& listener);

    // Destructor.
    ~TcpClient();

    // This client is connected to this TCP service.
    const sockaddr_in& getServerAddress() const;

    // This client's TCP port.
    unsigned short getClientPort() const;

    // Open a client connection to a local TCP service.
    void open(unsigned short port);

    // Open a client connection to a remote TCP service.
    void open(const sockaddr_in& address);

protected:
private:

    // Create an "accepted" client connection. This constructor
    // may only be called by TcpConnection.
    TcpClient(const sockaddr_in& far_address,
              const sockaddr_in& near_address,
#if defined(WIN32)
              unsigned short actual_port,
              SOCKET udp_socket,
              HANDLE udp_handle,
#else
              int udp_socket,
#endif
              unsigned long sequence_number,
              TcpServer& server,
              TcpConnectionListener& listener);

    // Don't allow default or copy constructors.
    TcpClient();
    TcpClient(const TcpClient&);

// Member data.
public:
protected:
private:

// Friends.
    friend class TcpConnection;
};

#endif
