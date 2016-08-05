#ifndef _H_APPCLIENT
#define _H_APPCLIENT

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
//  AppClient.h
//
// Description
//  Encapsulates a TcpClient object.
//
// RCS ID
// $Id: AppClient.h,v 1.6 2009/03/01 18:20:38 cwrapp Exp $
//
// CHANGE LOG
// $Log: AppClient.h,v $
// Revision 1.6  2009/03/01 18:20:38  cwrapp
// Preliminary v. 6.0.0 commit.
//
// Revision 1.5  2007/12/28 12:34:40  cwrapp
// Version 5.0.1 check-in.
//
// Revision 1.4  2005/05/28 13:31:18  cwrapp
// Updated C++ examples.
//
// Revision 1.0  2003/12/14 19:34:49  charlesr
// Initial revision
//

#include "TcpClient.h"
#include "TcpServer.h"
#include "TcpConnectionListener.h"
#include <string>

// Foward declarations.
class AppServer;

class AppClient :
    /* implements */ public TcpConnectionListener,
    /* implements */ public TimerListener
{
// Member functions.
public:

    // Default constructor.
    AppClient();

    // Create a client around an accepted connection.
    AppClient(const std::string& host,
              TcpClient& client,
              AppServer& owner);

    virtual ~AppClient();

    const std::string& getHost() const;

    // Create a TCP client object and open a connection to the
    // specified service.
    void open(const std::string& host,
              const sockaddr_in& address);

    // Close the TCP service.
    void close();

    // TCP connection listener callback methods.
    void opened(TcpConnection& connection);
    void openFailed(const char *reason,
                    TcpConnection& connection);
    void transmitted(TcpConnection& connection);
    void transmitFailed(const char *reason,
                        TcpConnection& connection);
    void receive(const char *data,
                 int size,
                 TcpConnection& connection);
    void halfClosed(TcpConnection& connection);
    void closed(const char *reason,
                TcpConnection& connection);

    // Clients never receive this event.
    void accepted(TcpClient&, TcpServer&) {};

    // Timer listener callback method.
    void handleTimeout(const char *name);

protected:
private:

    // Randomly set the transmit time.
    void setTransmitTimer();

// Member data.
public:
protected:
private:

    // The TCP service itself.
    TcpClient *_client_socket;

    // If this was an accepted client connection, _owner points
    // to the parent TCP service. This will be NULL otherwise.
    AppServer *_owner;

    // The host to which we are connected.
    std::string _host;

    // Every time a message is sent, increment this counter.
    int _messageCount;

    // Initialize the random number generator once per process.
    static int _initFlag;

    // Class constants. Minimum and Maximum transmit timeout.
    const static long MIN_SLEEP_TIME;
    const static long MAX_SLEEP_TIME;
};

#endif
