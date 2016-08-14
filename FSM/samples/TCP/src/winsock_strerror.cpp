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
// Copyright (C) 2000 - 2003 Charles W. Rapp.
// All Rights Reserved.
// 
// Contributor(s): 
//
// Function
//	Main
//
// Description
//  Encapsulates "TCP" server connection, accepting new client connections.
//
// RCS ID
// $Id: winsock_strerror.cpp,v 1.5 2009/03/01 18:20:38 cwrapp Exp $
//
// CHANGE LOG
// $Log: winsock_strerror.cpp,v $
// Revision 1.5  2009/03/01 18:20:38  cwrapp
// Preliminary v. 6.0.0 commit.
//
// Revision 1.4  2005/05/28 13:31:18  cwrapp
// Updated C++ examples.
//
// Revision 1.0  2003/12/14 19:44:45  charlesr
// Initial revision
//

#if defined(WIN32)
#include <winsock2.h>

//---------------------------------------------------------------
// winsock_strerror(int) (Routine)
// Return a string equivalent for a winsock error.
//
char* winsock_strerror(int error)
{
    char *retval;

    switch (error)
    {
        case WSANOTINITIALISED:
            retval = "winsock library not initialized";
            break;

        case WSAENETDOWN:
            retval = "network down";
            break;

        case WSAEAFNOSUPPORT:
            retval = "address family not supported";
            break;

        case WSAEINPROGRESS:
            retval = "blocking call in progress";
            break;

        case WSAEMFILE:
            retval = "no more socket descriptors available";
            break;

        case WSAENOBUFS:
            retval = "buffer space exhausted";
            break;

        case WSAEPROTONOSUPPORT:
            retval = "specified protocol not supported";
            break;

        case WSAEPROTOTYPE:
            retval = "specified protocol incorrect type for socket";
            break;

        case WSAESOCKTNOSUPPORT:
            retval = "socket family and type combination not supported";
            break;

        case WSAEADDRINUSE:
            retval = "address in use";
            break;

        case WSAEADDRNOTAVAIL:
            retval = "address not available";
            break;

        case WSAEFAULT:
            retval = "invalid address";
            break;

        case WSAEINVAL:
            retval = "invalid request";
            break;

        case WSAENOTSOCK:
            retval = "invalid socket specified";
            break;

        case WSAEISCONN:
            retval = "socket is connected";
            break;

        case WSAENETRESET:
            retval = "network reset";
            break;

        case WSAEOPNOTSUPP:
            retval = "operation not supported";
            break;

        case WSAESHUTDOWN:
            retval = "socket was shutdown";
            break;

        case WSAEWOULDBLOCK:
            retval = "socket would block";
            break;

        case WSAEMSGSIZE:
            retval = "message too large";
            break;

        case WSAETIMEDOUT:
            retval = "timed out";
            break;

        case WSAECONNRESET:
            retval = "connection reset";
            break;

        case WSAEINTR:
            retval = "call interrupted";
            break;

        case WSAEHOSTUNREACH:
            retval = "host unreachable";
            break;

        case WSAECONNABORTED:
            retval = "connection aborted";
            break;

        case WSAEDESTADDRREQ:
            retval = "destination address required";
            break;

        case WSAENETUNREACH:
            retval = "network unreachable";
            break;

        case WSASYSNOTREADY:
            retval = "network subsystem not ready";
            break;

        case WSAVERNOTSUPPORTED:
            retval = "version not supported";
            break;

        case WSAEPROCLIM:
            retval = "process limit reached";
            break;

        case WSAHOST_NOT_FOUND:
            retval = "host not found";
            break;

        case WSATRY_AGAIN:
            retval = "try again";
            break;

        case WSANO_RECOVERY:
            retval = "no recovery";
            break;

        case WSANO_DATA:
            retval = "no data";
            break;

        default:
            retval = "unknown error code";
            break;
    }

    return(retval);
} // end of TcpConnection::winsock_strerror(int)

#endif
