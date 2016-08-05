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
// File
//  client.cpp
//
// Description
//  Encapsulates "TCP" client connection.
//
// RCS ID
// $Id: client.cpp,v 1.6 2007/12/28 12:34:40 cwrapp Exp $
//
// CHANGE LOG
// $Log: client.cpp,v $
// Revision 1.6  2007/12/28 12:34:40  cwrapp
// Version 5.0.1 check-in.
//
// Revision 1.5  2005/06/08 11:09:12  cwrapp
// + Updated Python code generator to place "pass" in methods with empty
//   bodies.
// + Corrected FSM errors in Python example 7.
// + Removed unnecessary includes from C++ examples.
// + Corrected errors in top-level makefile's distribution build.
//
// Revision 1.4  2005/05/28 13:31:18  cwrapp
// Updated C++ examples.
//
// Revision 1.0  2003/12/14 19:43:52  charlesr
// Initial revision
//

#include "Eventloop.h"
#include "AppClient.h"
#if (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#elif defined(WIN32)
#include <winsock2.h>
#else
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif
#include <signal.h>
#include <string>

using namespace std;

#ifndef WIN32
#ifndef SA_NOMASK
#define SA_NOMASK 0
#endif
#endif

// Global variable declarations.
Eventloop *Gevent_loop;

// Static variable declarations.
static AppClient *Sclient_socket;

// Constant declarations.
const static int STDIN_FD = 0;
#ifndef INADDR_NONE
const static unsigned long INADDR_NONE = 0xffffffff;
#endif

#if defined(WIN32)
char* winsock_strerror(int);
#endif

//---------------------------------------------------------------
// main(int, char**) (Routine)
// Process the command line arguments, open the TCP service and
// then sit in the event loop. Stop running when the Enter key
// is hit.
//
int main(int argc, char *argv[])
{
    int longPort;
    unsigned long ip_address = INADDR_ANY;
    sockaddr_in address;
    int retcode;
#if defined(WIN32)
    WORD winsockVersion;
    WSADATA winsockData;
    int errorCode;
#else
	struct sigaction signalAction;
#endif

	// External routine declarations.
    unsigned long getIPAddress(const char*);
	void sigintHandler(int);
#if defined(WIN32)
    char* winsock_strerror(int);
#endif

#ifdef WIN32
    // Windows kinda supports signals.
    (void) signal(SIGINT, sigintHandler);
#else
    // Set up the SIGINT handler.
    signalAction.sa_handler = sigintHandler;
#if defined(__hpux) || defined (__linux__)
    sigemptyset(&signalAction.sa_mask);
#if defined(__linux__)
    signalAction.sa_restorer = NULL;
#endif
#endif
    signalAction.sa_flags = SA_NOMASK;
    if (sigaction(SIGINT,
        &signalAction,
        (struct sigaction *) NULL) != 0)
    {
        cerr << "Unable to set SIGINT handling function." << endl;
        exit(1);
    }
#endif

#if defined(WIN32)
    // Initialize winsock.
    winsockVersion = MAKEWORD(2, 0);
    if ((errorCode = WSAStartup(winsockVersion, &winsockData)) != 0)
    {
        cout << "Unable to initialize Win32 sockets - "
            << winsock_strerror(errorCode)
            << "."
            << endl;
        exit(2);
    }
#endif

    if (argc != 3)
    {
        cerr << argv[0]
             << ": Wrong number of arguments."
             << endl;
        cerr << "usage: client {hostname | IP address} port" << endl;
        retcode = 1;
    }
    // Check the port's validity.
    else if (sscanf(argv[2], "%d", &longPort) != 1 ||
             longPort < 0 ||
             longPort > 65535)
    {
        cerr << "Invalid port number - \""
             << argv[2]
             << "\"."
             << endl;
        retcode = 2;
    }
    // Check the address' validity.
    else if ((ip_address = getIPAddress(argv[1])) == INADDR_NONE)
    {
        cerr << "Invalid host - \""
             << argv[1]
             << "\"."
             << endl;
        retcode = 3;
    }
    else
    {
        string host(argv[1]);
        unsigned short port;

        // Set up the service address.
        (void) memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        port = static_cast<unsigned short>(longPort);
        address.sin_port = htons(port);
        address.sin_addr.s_addr = ip_address;

        cout << "(Starting execution. Hit \"Cntl-c\" to stop.)" << endl;
        
        // 1. Create the event loop object.
        Gevent_loop = new Eventloop();

        // 2. Open server port.
        Sclient_socket = new AppClient();
        Sclient_socket->open(host, address);
        
        // 3. Start connection process.
        Gevent_loop->start();

        cout << "(Stopping execution.)" << endl;

        // 4. Close connection.
        delete Sclient_socket;
        Sclient_socket = NULL;

        // Delete the event loop.
        delete Gevent_loop;
        Gevent_loop = NULL;

#if defined(WIN32)
        WSACleanup();
#endif

        retcode = 0;
    }

    return(retcode);
} // end of main(int, char**)

//---------------------------------------------------------------
// socketClosed() (Routine)
// The server socket is closed. Stop the application.
//
void socketClosed()
{
    Gevent_loop->stop();
    return;
} // end of socketClosed()

//---------------------------------------------------------------
// getIPAddress(const char*) (Routine)
// Convert a host name or dotted-decimal notation address into
// an IP address.
//
unsigned long getIPAddress(const char *host)
{
    hostent *host_entry = NULL;
    unsigned long retcode;

    // First try to convert host name as a dotted-decimal
    // notation. If that fails, try gethostbyname().
    if ((retcode = inet_addr(host)) == INADDR_NONE &&
        (host_entry = gethostbyname(host)) == NULL)
    {
        // Failed to convert host name into IP address.
        retcode = INADDR_NONE;
    }
    else if (host_entry != NULL)
    {
        (void) memcpy(&retcode,
                      host_entry->h_addr,
                      host_entry->h_length);
    }

    return(retcode);
} // end of getIPAddress(const char*)

//---------------------------------------------------------------
// sigintHandler(int) (Routine)
// When an interrupt is detected, start shutting down the
// connection.
//
void sigintHandler(int)
{
    Sclient_socket->close();

#ifdef WIN32
    // Windows removes the SIGINT callback. So put
    // the callback back in place.
    (void) signal(SIGINT, sigintHandler);
#endif

	return;
} // end of sigintHandler(int)
