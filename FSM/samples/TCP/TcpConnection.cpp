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
//  TcpConnection.cpp
//
// Description
//  TCP connection class implementation.
//
// RCS ID
// $Id: TcpConnection.cpp,v 1.9 2015/08/02 19:44:35 cwrapp Exp $
//
// CHANGE LOG
// $Log: TcpConnection.cpp,v $
// Revision 1.9  2015/08/02 19:44:35  cwrapp
// Release 6.6.0 commit.
//
// Revision 1.8  2014/09/06 09:04:29  fperrad
// pragma only for MS compiler
//
// Revision 1.7  2007/12/28 12:34:40  cwrapp
// Version 5.0.1 check-in.
//
// Revision 1.6  2005/06/08 11:09:12  cwrapp
// + Updated Python code generator to place "pass" in methods with empty
//   bodies.
// + Corrected FSM errors in Python example 7.
// + Removed unnecessary includes from C++ examples.
// + Corrected errors in top-level makefile's distribution build.
//
// Revision 1.5  2005/05/28 13:31:18  cwrapp
// Updated C++ examples.
//
// Revision 1.0  2003/12/14 19:40:23  charlesr
// Initial revision
//

#ifdef _MSC_VER
#pragma warning(disable: 4355)
#endif

#include "TcpConnection.h"
#include "TcpSegment.h"
#include "TcpClient.h"
#include "TcpServer.h"
#include "Eventloop.h"
#include <errno.h>
#include <string.h>
#include <memory.h>
#include <fcntl.h>
#if !defined(WIN32)
#include <unistd.h>
#include <netdb.h>
#  if defined(__sun)
#include <sys/systeminfo.h>
#  endif
#endif

#ifdef _MSC_VER
// Get rid of an annoying build warning.
#pragma warning(disable: 4355)
#endif

using namespace std;

#if defined(WIN32)
typedef int socklen_t;
#endif

// External variable declarations.
extern Eventloop *Gevent_loop;

// Const member declarations.
const unsigned long TcpConnection::ISN = 1415531521;
const int TcpConnection::MIN_TIMEOUT = 1;
const int TcpConnection::ACK_TIMEOUT = 2000;
const int TcpConnection::CLOSE_TIMEOUT = 5000;
const int TcpConnection::BUFFER_BLOCK_SIZE = 4096;

// Const declarations.
const static int MSG_READ = 0;
const static int NO_SEND_FLAGS = 0;
const static long MAX_HOSTNAME_LEN = 257;
const static unsigned long LOCAL_HOST_ADDRESS = 0x7f000001;
#if defined(WIN32)
const static unsigned short MIN_PORT = 1024;
const static unsigned short MAX_PORT = 5000;
#endif

// External routine declarations.
char* winsock_strerror(int);

//---------------------------------------------------------------
// getFarAddress() const (Public)
// Return the far-end's 4-byte IP address.
//
unsigned long TcpConnection::getFarAddress() const
{
    return (_farAddress.sin_addr.s_addr);
} // end of TcpConnection::getFarAddress() const

//---------------------------------------------------------------
// getFarPort() const (Public)
// Return the far-end's TCP port.
//
unsigned short TcpConnection::getFarPort() const
{
    return (_farAddress.sin_port);
} // end of TcpConnection::getFarPort() const

//---------------------------------------------------------------
// getSequenceNumber() const (Public)
// Return the current sequence number.
//
unsigned long TcpConnection::getSequenceNumber() const
{
    return (_sequence_number);
} // end of TcpConnection::getSequenceNumber() const

//---------------------------------------------------------------
// transmit(const unsigned char*, int, int) (Public)
// Transmit data to the far side.
//
void TcpConnection::transmit(const char *data,
                             int offset,
                             int size)
{
#ifdef CRTP
    Transmit(data, offset, size);
#else
    _fsm.Transmit(data, offset, size);
#endif
    return;
} // end of TcpConnection::transmit(const unsigned char*, int, int)

//---------------------------------------------------------------
// doClose() (Public)
// Start closing this connection.
//
void TcpConnection::doClose()
{
#ifdef CRTP
    Close();
#else
    _fsm.Close();
#endif
    return;
} // end of TcpConnection::doClose()

//---------------------------------------------------------------
// setListener(TcpConnectionListener&) (Public)
// Set the connection's listener to a new object.
//
void TcpConnection::setListener(TcpConnectionListener& listener)
{
    _listener = &listener;
    return;
} // end of TcpConnection::setListener(TcpConnectionListener&)

//---------------------------------------------------------------
// handleReceive(int) (Public)
// Process incoming data.
//
void TcpConnection::handleReceive(int)
{
    sockaddr sourceAddress;
    TcpSegment *segment;
    socklen_t addressSize;
    int flags,
        done,
        errorFlag,
        bytesRead,
        totalBytesRead;

    // Read in the segment. Since the data may exceed the buffer
    // size, keep expanding the buffer until all the data is
    // read.
    for (done = 0, bytesRead = 0, totalBytesRead = 0, errorFlag = 0;
         done == 0;
         totalBytesRead += bytesRead)
    {
        // Be certain to offset properly into the input buffer.
        addressSize = sizeof(sourceAddress);
        bytesRead = recvfrom(_udp_socket,
                             (_buffer + totalBytesRead),
                             _bufferSize,
                             MSG_READ,
                             &sourceAddress,
                             &addressSize);

#if defined(WIN32)
        if (bytesRead == SOCKET_ERROR)
#else
        if (bytesRead < 0)
#endif
        {
            done = 1;

            if (errno != EAGAIN)
            {
                cerr << "Receive failure - "
                     << strerror(errno)
                     << "."
                     << endl;

                errorFlag = 1;
            }
            else
            {
                bytesRead = 0;
            }
        }
        else if (bytesRead >= 0 && bytesRead < _bufferSize)
        {
            done = 1;
        }
        else
        {
            // The entire buffer was filled with the data.
            // Expand the buffer and read some more.
            expandBuffer();
        }
    }

    if (errorFlag == 0 && bytesRead > 0)
    {
        segment = new TcpSegment(reinterpret_cast<sockaddr_in&>(sourceAddress),
                                 _nearAddress,
                                 _buffer,
                                 totalBytesRead);

#if defined(FSM_DEBUG)
        cout << "**** Received segment:"
             << endl
             << *segment
             << endl;
#endif

        // Generate the appropriate transition based on the header
        // flags.
        flags = segment->getFlags();
        switch(flags)
        {
            case TcpSegment::FIN:
#ifdef CRTP
                FIN(*segment);
#else
                _fsm.FIN(*segment);
#endif
                break;

            case TcpSegment::SYN:
#ifdef CRTP
                SYN(*segment);
#else
                _fsm.SYN(*segment);
#endif
                break;

            case TcpSegment::RST:
#ifdef CRTP
                RST(*segment);
#else
                _fsm.RST(*segment);
#endif
                break;

            case TcpSegment::PSH:
#ifdef CRTP
                PSH(*segment);
#else
                _fsm.PSH(*segment);
#endif
                break;

            case TcpSegment::ACK:
#ifdef CRTP
                ACK(*segment);
#else
                _fsm.ACK(*segment);
#endif
                break;

            case TcpSegment::URG:
#ifdef CRTP
                URG(*segment);
#else
                _fsm.URG(*segment);
#endif
                break;

            case TcpSegment::FIN_ACK:
#ifdef CRTP
                FIN_ACK(*segment);
#else
                _fsm.FIN_ACK(*segment);
#endif
                break;

            case TcpSegment::SYN_ACK:
#ifdef CRTP
                SYN_ACK(*segment);
#else
                _fsm.SYN_ACK(*segment);
#endif
                break;

            case TcpSegment::RST_ACK:
#ifdef CRTP
                RST_ACK(*segment);
#else
                _fsm.RST_ACK(*segment);
#endif
                break;

            case TcpSegment::PSH_ACK:
#ifdef CRTP
                PSH_ACK(*segment);
#else
                _fsm.PSH_ACK(*segment);
#endif
                break;

            default:
#ifdef CRTP
                UNDEF(*segment);
#else
                _fsm.UNDEF(*segment);
#endif
                break;
        }
    }

    return;
} // end of TcpConnection::handleReceive(int)

//---------------------------------------------------------------
// handleTimeout(const char*) (Public)
// Generate the appropriate transition based on the timer's name.
//
void TcpConnection::handleTimeout(const char *name)
{
#if defined(FSM_DEBUG)
    cout << "**** Timer " << name << " has expired." << endl;
#endif

    if (strcmp(name, "ACK_TIMER") == 0)
    {
#ifdef CRTP
        AckTimeout();
#else
        _fsm.AckTimeout();
#endif
    }
    else if (strcmp(name, "CONN_ACK_TIMER") == 0)
    {
#ifdef CRTP
        ConnAckTimeout();
#else
        _fsm.ConnAckTimeout();
#endif
    }
    else if (strcmp(name, "TRANS_ACK_TIMER") == 0)
    {
#ifdef CRTP
        TransAckTimeout();
#else
        _fsm.TransAckTimeout();
#endif
    }
    else if (strcmp(name, "CLOSE_ACK_TIMER") == 0)
    {
#ifdef CRTP
        CloseAckTimeout();
#else
        _fsm.CloseAckTimeout();
#endif
    }
    else if (strcmp(name, "CLOSE_TIMER") == 0)
    {
#ifdef CRTP
        CloseTimeout();
#else
        _fsm.CloseTimeout();
#endif
    }
    else if (strcmp(name, "SERVER_OPENED") == 0)
    {
#ifdef CRTP
        ServerOpened();
#else
        _fsm.ServerOpened();
#endif
    }
    else if (strcmp(name, "CLIENT_OPENED") == 0)
    {
#ifdef CRTP
        ClientOpened(&_farAddress);
#else
        _fsm.ClientOpened(&_farAddress);
#endif
    }
    else if (strcmp(name, "OPEN_FAILED") == 0)
    {
#ifdef CRTP
        OpenFailed(_errorMessage);
#else
        _fsm.OpenFailed(_errorMessage);
#endif
        if (_errorMessage != NULL)
        {
            delete[] _errorMessage;
            _errorMessage = NULL;
        }
    }

    return;
} // end of TcpConnection::handleTimeout(const char*)

//---------------------------------------------------------------
// openServerSocket(unsigned short) (Public)
// Open a server socket on the specified port.
//
void TcpConnection::openServerSocket(unsigned short port)
{
#if defined(WIN32)
    unsigned long blockingFlag = 1;
    HANDLE newHandle;
#endif

    // Set up this server's address.
    (void) memset(&_nearAddress, 0, sizeof(_nearAddress));
    _nearAddress.sin_family = AF_INET;
    _nearAddress.sin_port = port;
    _nearAddress.sin_addr.s_addr = INADDR_ANY;

    // Tell the UDP socket to route data bound for port to this
    // object.
#if defined(WIN32)
    if ((_udp_win_socket = socket(AF_INET, SOCK_DGRAM, 0))
            == INVALID_SOCKET ||
        DuplicateHandle(GetCurrentProcess(),
                        (HANDLE) _udp_win_socket,
                        GetCurrentProcess(),
                        &newHandle,
                        0,
                        FALSE,
                        DUPLICATE_SAME_ACCESS) == FALSE ||
        bind((unsigned int) newHandle,
             reinterpret_cast<sockaddr*>(&_nearAddress),
             sizeof(_nearAddress))
            == SOCKET_ERROR)
#else
    if ((_udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ||
        bind(_udp_socket,
             reinterpret_cast<sockaddr*>(&_nearAddress),
             sizeof(_nearAddress))
            < 0)
#endif
    {
        char *error;

        // Use timer to report this event asynchronously.
        // Store away error message.
        if (_errorMessage != NULL)
        {
            delete[] _errorMessage;
            _errorMessage = NULL;
        }
#if defined(WIN32)
        error = winsock_strerror(WSAGetLastError());
#else
        error = strerror(errno);
#endif
        _errorMessage = new char[strlen(error) + 1];
        (void) strcpy(_errorMessage, error);
        Gevent_loop->startTimer("OPEN_FAILED",
                                MIN_TIMEOUT,
                                *this);
    }
    else
    {
        // Get the UDP socket's address.
        _nearAddress.sin_family = AF_INET;
        _nearAddress.sin_addr.s_addr = getLocalAddress();

        // Set the socket to non-blocking and register it with
        // the event loop.
#if defined(WIN32)
        _udp_socket = (unsigned int) newHandle;
        (void) ioctlsocket(_udp_socket, FIONBIO, &blockingFlag);
#else
        (void) fcntl(_udp_socket, F_SETFL, O_NDELAY);
#endif
        Gevent_loop->addFD(_udp_socket, *this);

        Gevent_loop->startTimer("SERVER_OPENED",
                                MIN_TIMEOUT,
                                *this);
    }

    return;
} // end of TcpConnection::openServerSocket(int)

//---------------------------------------------------------------
// openClientSocket(const sockaddr_in*) (Public)
// Connect to the specified address.
//
void TcpConnection::openClientSocket(const sockaddr_in *address)
{
#if defined(WIN32)
    unsigned long blockingFlag = 1;
    HANDLE newHandle;
    int new_port;
#endif

    // Set up this client's address.
    (void) memset(&_nearAddress, 0, sizeof(_nearAddress));
    _nearAddress.sin_family = AF_INET;
    _nearAddress.sin_port = 0;
    _nearAddress.sin_addr.s_addr = INADDR_ANY;

#if defined(WIN32)
    if ((_udp_win_socket = socket(AF_INET, SOCK_DGRAM, 0))
            == INVALID_SOCKET ||
        DuplicateHandle(GetCurrentProcess(),
                        (HANDLE) _udp_win_socket,
                        GetCurrentProcess(),
                        &newHandle,
                        0,
                        FALSE,
                        DUPLICATE_SAME_ACCESS) == FALSE ||
        (new_port = doBind((int) newHandle)) < 0)
#else
    if ((_udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ||
        bind(_udp_socket,
             reinterpret_cast<sockaddr*>(&_nearAddress),
             sizeof(_nearAddress))
            < 0)
#endif
    {
        char *error;

        // Use timer to report this event asynchronously.
        // Store away error message.
        if (_errorMessage != NULL)
        {
            delete[] _errorMessage;
            _errorMessage = NULL;
        }
#if defined(WIN32)
        error = winsock_strerror(WSAGetLastError());
#else
        error = strerror(errno);
#endif
        _errorMessage = new char[strlen(error) + 1];
        (void) strcpy(_errorMessage, error);
        Gevent_loop->startTimer("OPEN_FAILED",
                                MIN_TIMEOUT,
                                *this);
    }
    else
    {
        // Get the UDP socket's address.
        _nearAddress.sin_family = AF_INET;
#if defined(WIN32)
        _nearAddress.sin_port = (unsigned short) new_port;
#else
        _nearAddress.sin_port = getLocalPort(_udp_socket);
#endif
        _nearAddress.sin_addr.s_addr = getLocalAddress();

        // Set the socket to non-blocking and register it with
        // the event loop.
#if defined(WIN32)
        _udp_socket = (unsigned int) newHandle;
        (void) ioctlsocket(_udp_socket, FIONBIO, &blockingFlag);
#else
        (void) fcntl(_udp_socket, F_SETFL, O_NDELAY);
#endif
        Gevent_loop->addFD(_udp_socket, *this);

        // Save the far-end address for later use.
        (void) memcpy(&_farAddress, address, sizeof(_farAddress));

        _sequence_number = ISN;

        Gevent_loop->startTimer("CLIENT_OPENED",
                                MIN_TIMEOUT,
                                *this);
    }

    return;
} // end of TcpConnection::openClientSocket(const sockaddr_in*)

//---------------------------------------------------------------
// openSuccess() (Public)
// The socket has been successfully opened.
//
void TcpConnection::openSuccess()
{
    _listener->opened(*this);
    return;
} // end of TcpConnection::openSuccess()

//---------------------------------------------------------------
// openFailed(const char*) (Public)
// The socket could not be opened.
//
void TcpConnection::openFailed(const char *reason)
{
    _listener->openFailed(reason, *this);
    return;
} // end of TcpConnection::openFailed(const char*)

//---------------------------------------------------------------
// closeSocket() (Public)
// Finish closing the socket.
//
void TcpConnection::closeSocket()
{
    if (_udp_socket >= 0)
    {
        // Remove this UDP socket from the event loop.
        Gevent_loop->removeFD(_udp_socket);

        // Now close the UDP socket.
#if defined(WIN32)
        CloseHandle((HANDLE) _udp_socket);
        closesocket(_udp_win_socket);
        _udp_win_socket = NULL;
#else
        close(_udp_socket);
#endif
        _udp_socket = -1;
    }
    
    return;
} // end of TcpConnection::closeSocket()

//---------------------------------------------------------------
// halfClosed() (Public)
// The far end client socket has closed its half of the
// connection.
//
void TcpConnection::halfClosed()
{
    _listener->halfClosed(*this);
    return;
} // end of TcpConnection::halfClosed()

//---------------------------------------------------------------
// closed(const char*) (Public)
// The connection is completely closed.
//
void TcpConnection::closed(const char *reason)
{
    TcpConnectionListener *listener;

    if (_listener != NULL)
    {
        listener = _listener;
        _listener = NULL;
        listener->closed(reason, *this);
    }

    return;
} // end of TcpConnection::closed(const char*)

//---------------------------------------------------------------
// clearListener() (Public)
// Clear this connection's current listener.
//
void TcpConnection::clearListener()
{
    _listener = NULL;
    return;
} // end of TcpConnection::clearListener()

//---------------------------------------------------------------
// transmitted() (Public)
// Data transmission successfully completed.
//
void TcpConnection::transmitted()
{
    _listener->transmitted(*this);
    return;
} // end of TcpConnection::transmitted()

//---------------------------------------------------------------
// transmitFailed(const char*) (Public)
// Data transmission failed.
//
void TcpConnection::transmitFailed(const char *reason)
{
    _listener->transmitFailed(reason, *this);
    return;
} // end of TcpConnection::transmitFailed(const char*)

//---------------------------------------------------------------
// receive(const TcpSegment&) (Public)
// Send received data to the listener.
//
void TcpConnection::receive(const TcpSegment& segment)
{
    if (_listener != NULL)
    {
        _listener->receive(segment.getData(),
                           segment.getDataSize(),
                           *this);
    }

    return;
} // end of TcpConnection::receive(const TcpSegment&)

//---------------------------------------------------------------
// sendOpenSyn(const sockaddr_in*) (Public)
// Send the opening SYN message which requests connection to a
// service.
//
void TcpConnection::sendOpenSyn(const sockaddr_in *address)
{
    TcpSegment segment(*address,
                       _nearAddress,
                       0,
                       0,
                       TcpSegment::SYN,
                       NULL,
                       0,
                       0);

    doSend(TcpSegment::SYN, NULL, 0, 0, &segment);
    return;
} // end of TcpConnection::sendOpenSyn(const sockaddr_in*)

//---------------------------------------------------------------
// accept(const TcpSegment&) (Public)
// Create a client socket to handle the new connection.
//
void TcpConnection::accept(const TcpSegment& segment)
{
    TcpClient *accept_client;
    int new_socket;
#if defined(WIN32)
    unsigned long blockingFlag = 1;
    HANDLE newHandle;
    int new_port;
#else
    sockaddr_in clientAddress;
#endif

    // Create a new UDP socket to handle this connection.
    // Let it be assigned a random UDP port.
#if defined(WIN32)
    newHandle = NULL;
    if ((new_socket = socket(AF_INET, SOCK_DGRAM, 0))
            == INVALID_SOCKET ||
        DuplicateHandle(GetCurrentProcess(),
                        (HANDLE) new_socket,
                        GetCurrentProcess(),
                        &newHandle,
                        0,
                        FALSE,
                        DUPLICATE_SAME_ACCESS) == FALSE ||
        (new_port = doBind((int) newHandle)) < 0)
    {
        // If an error occurs now, ignore it.
        if (new_socket != INVALID_SOCKET)
        {
            closesocket(new_socket);
        }
        if (newHandle != NULL)
        {
            CloseHandle((HANDLE) newHandle);
        }
    }
    else
    {
        // Set the socket to non-blocking.
        (void) ioctlsocket(
            (unsigned int) newHandle, FIONBIO, &blockingFlag);

        // Have the new client socket use this server
        // socket's near address for now.
        accept_client = new TcpClient(segment.getSource(),
                                      _nearAddress,
                                      (unsigned short) new_port,
                                      new_socket,
                                      newHandle,
                                      _sequence_number,
                                      (TcpServer&) *this,
                                      *_listener);
#else
    // Set up this client's address.
    (void) memset(&clientAddress, 0, sizeof(clientAddress));
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = 0;
    clientAddress.sin_addr.s_addr = INADDR_ANY;

    if ((new_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ||
        bind(new_socket,
             reinterpret_cast<sockaddr*>(&clientAddress),
             sizeof(clientAddress))
            < 0)
    {
        // If an error occurs now, ignore it.
        if (new_socket >= 0)
        {
            close(new_socket);
        }
    }
    else
    {
        // Set the socket to non-blocking.
        (void) fcntl(new_socket, F_SETFL, O_NDELAY);

        // Have the new client socket use this server
        // socket's near address for now.
        accept_client = new TcpClient(segment.getSource(),
                                      _nearAddress,
                                      new_socket,
                                      _sequence_number,
                                      (TcpServer&) *this,
                                      *_listener);

#endif

        accept_client->acceptOpen(segment);
    }

    return;
} // end of TcpConnection::accept(const TcpSegment&)

//---------------------------------------------------------------
// accepted() (Public)
// Connection completely accepted.
//
void TcpConnection::accepted()
{
    TcpServer *server = _server;
    TcpConnectionListener *listener = _listener;

    // Tell the server listener that a new connection has
    // been accepted. Then clear the server listener because
    // this socket is now truly a client socket. Clear the
    // listener member data now because the callback method
    // will be resetting it and the reset will fail if we
    // don't do it.
    _server = NULL;
    _listener = NULL;
    listener->accepted((TcpClient&) *this, *server);

    return;
} // end of TcpConnection::accepted()

//---------------------------------------------------------------
// sendSynAck(const TcpSegment&) (Public)
// Acknowledge a SYN message.
//
void TcpConnection::sendSynAck(const TcpSegment& segment)
{
    unsigned short port;
    char port_bytes[2];

    // Tell the far end client socket with what port it should
    // now communicate.
#if defined(WIN32)
    port = _actualPort;
#else
    port = getLocalPort(_udp_socket);
#endif

    port_bytes[0] = (char) (port & 0x00ff);
    port_bytes[1] = (char) ((port & 0xff00) >> 8);

    doSend(TcpSegment::SYN_ACK, port_bytes, 0, 2, &segment);

    return;
} // end of TcpConnection::sendSynAck(const TcpSegment&)

//---------------------------------------------------------------
// sendSynAckAck(const TcpSegment&) (Public)
// Acknowledge the service's acknowledge. Need to do this so
// doSend() will use the correct address.
//
void TcpConnection::sendSynAckAck(const TcpSegment& segment)
{
    TcpSegment faux_segment(_farAddress,
                            _nearAddress,
                            segment.getSequenceNumber(),
                            segment.getAcknowledgeNumber(),
                            segment.getFlags(),
                            NULL,
                            0,
                            0);

    doSend(TcpSegment::ACK, NULL, 0, 0, &faux_segment);
    return;
} // end of TcpConnection::sendSynAckAck(const TcpSegment&)

//---------------------------------------------------------------
// doSend(...) (Public)
// Send data to the specified address.
//
void TcpConnection::doSend(unsigned short flags,
                           const char *data,
                           int offset,
                           int size,
                           const TcpSegment *recv_segment)
{
    sockaddr_in to_address;
    unsigned long ack_number;
    TcpSegment *send_segment;
    char *packet;
    int packet_size;

    (void) memset(&to_address, 0, sizeof(to_address));

    // First, use the received segment.
    // Else, use the connection's far end.
    if (recv_segment != NULL)
    {
        (void) memcpy(&to_address,
                      &(recv_segment->getSource()),
                      sizeof(to_address));
    }
    // If the address was not specified, then send this segment
    // to whatever client socket we are currently speaking.
    else
    {
        (void) memcpy(&to_address, &_farAddress, sizeof(to_address));
    }

    // Send the ack number only if the ack flag is set.
    if ((flags & TcpSegment::ACK) == 0 || recv_segment == NULL)
    {
        ack_number = 0;
    }
    else
    {
        // Figure out the ack number based on the received
        // segment's sequence number and data size.
        ack_number = getAck(*recv_segment);
    }

    send_segment =
        new TcpSegment(_nearAddress,
                       to_address,
                       _sequence_number,
                       ack_number,
                       flags,
                       data,
                       offset,
                       size);

    // Advance the sequence number depending on the message sent.
    // Don't do this if the message came from an interloper.
    if (recv_segment == NULL ||
        ((recv_segment->getSource()).sin_port == _farAddress.sin_port &&
         (recv_segment->getSource()).sin_addr.s_addr == _farAddress.sin_addr.s_addr))
    {
        _sequence_number = getAck(*send_segment);
    }

    // Now send the data. First convert the TCP segment into a
    // sequence of raw bytes.
    send_segment->packetize(packet, packet_size);

    if (packet != NULL && packet_size > 0)
    {
#if defined(FSM_DEBUG)
        cout << "**** Transmit segment:\n" << *send_segment << endl;
#endif

#if defined(WIN32)
        if (sendto(_udp_socket,
                   packet,
                   packet_size,
                   NO_SEND_FLAGS,
                   reinterpret_cast<sockaddr *>(&to_address),
                   sizeof(to_address)) == SOCKET_ERROR)
#else
        if (sendto(_udp_socket,
                   packet,
                   packet_size,
                   NO_SEND_FLAGS,
                   reinterpret_cast<sockaddr *>(&to_address),
                   sizeof(to_address)) < 0)
#endif
            {
                cerr << "Transmit failed - "
                     << strerror(errno)
                     << "."
                     << endl;
            }

        // Free up the allocated packet.
        delete[] packet;
    }

    delete send_segment;

    return;
} // end of TcpConnection::doSend(...)

//---------------------------------------------------------------
// startTimer(const char*, long) (Public)
// Start the named timer running.
//
void TcpConnection::startTimer(const char *name, long duration)
{
#if defined(FSM_DEBUG)
    cout << "**** Starting timer "
         << name
         << ", duration: "
         << duration
         << " milliseconds."
         << endl;
#endif

    Gevent_loop->startTimer(name, duration, *this);
    return;
} // end of TcpConnection::startTimer(const char*, long)

//---------------------------------------------------------------
// stopTimer(const char*) (Public)
// Stop the named timer.
//
void TcpConnection::stopTimer(const char* name)
{
#if defined(FSM_DEBUG)
    cout << "**** Stopping timer " << name << "." << endl;
#endif

    Gevent_loop->stopTimer(name, *this);
    return;
} // end of TcpConnection::stopTimer(const char*)

//---------------------------------------------------------------
// setNearAddress() (Public)
// Figure out this socket's port and address based on the socket.
//
void TcpConnection::setNearAddress()
{
    // Get the UDP socket's address.
    _nearAddress.sin_port = AF_INET;
#if defined(WIN32)
    _nearAddress.sin_port = _actualPort;
#else
    _nearAddress.sin_port = getLocalPort(_udp_socket);
#endif
    _nearAddress.sin_addr.s_addr = getLocalAddress();

    return;
} // end of TcpConnection::setNearAddress()

//---------------------------------------------------------------
// setFarAddress(const TcpSegment&) (Public)
// Send data to a new client port instead of the server port.
//
void TcpConnection::setFarAddress(const TcpSegment& segment)
{
    const char *data = segment.getData();
    unsigned short port;

    _farAddress.sin_family = AF_INET;
    port = ((((unsigned short) data[0]) & 0x00ff) |
            ((((unsigned short) data[1]) & 0x00ff) << 8));
    _farAddress.sin_port = port;
    _farAddress.sin_addr.s_addr =
        segment.getSource().sin_addr.s_addr;

    return;
} // end of TcpConnection::setFarAddress(const TcpSegment&)

//---------------------------------------------------------------
// deleteSegment(const TcpSegment&) (Public)
// Delete a segment object.
//
void TcpConnection::deleteSegment(const TcpSegment& segment)
{
    delete const_cast<TcpSegment*>(&segment);
    return;
} // end of TcpConnection::deleteSegment(const TcpSegment&)

//---------------------------------------------------------------
// TcpConnection(TcpConnectionListener&) (Protected)
// Create a server socket.
//
TcpConnection::TcpConnection(TcpConnectionListener& listener)
: _listener(&listener),
#if defined(WIN32)
  _actualPort(0),
  _udp_win_socket(NULL),
#endif
  _udp_socket(-1),
  _sequence_number(ISN),
  _buffer(NULL),
  _bufferSize(0),
  _server(NULL),
  _errorMessage(NULL)
#ifdef CRTP
#else
  , _fsm(*this)
#endif
{
    (void) memset(&_nearAddress, 0, sizeof(_nearAddress));
    (void) memset(&_farAddress, 0, sizeof(_farAddress));
    expandBuffer();

#if defined(FSM_DEBUG)
    // Turn on state map debug printout.
#ifdef CRTP
    setDebugFlag(true);
#else
    _fsm.setDebugFlag(true);
#endif
#endif

    return;
} // end of TcpConnection::TcpConnection(TcpConnectionListener&)

//---------------------------------------------------------------
// TcpConnection(const sockaddr_in&, ...) (Protected)
// "Accepted" client connection.
//
TcpConnection::TcpConnection(const sockaddr_in& far_address,
                             const sockaddr_in& near_address,
#if defined(WIN32)
                             unsigned short actual_port,
                             SOCKET udp_socket,
                             HANDLE udp_handle,
#else
                             int udp_socket,
#endif
                             int sequence_number,
                             TcpServer& server,
                             TcpConnectionListener& listener)
: _listener(&listener),
#if defined(WIN32)
  _actualPort(actual_port),
  _udp_win_socket(udp_socket),
  _udp_socket((unsigned int) udp_handle),
#else
  _udp_socket(udp_socket),
#endif
  _sequence_number(sequence_number),
  _buffer(NULL),
  _bufferSize(0),
  _server(&server),
  _errorMessage(NULL)
#ifdef CRTP
#else
  , _fsm(*this)
#endif
{
    // Register the UDP socket with the event loop.
    Gevent_loop->addFD(_udp_socket, *this);

    (void) memcpy(&_nearAddress, &near_address, sizeof(_nearAddress));
    (void) memcpy(&_farAddress, &far_address, sizeof(_farAddress));

    // Set up the input buffer.
    expandBuffer();

#if defined(FSM_DEBUG)
    // Turn on state map debug printout.
#ifdef CRTP
    setDebugFlag(true);
#else
    _fsm.setDebugFlag(true);
#endif
#endif

    return;
} // end of TcpConnection::TcpConnection(...)

//---------------------------------------------------------------
// ~TcpConnection() (Protected)
// Destructor.
//
TcpConnection::~TcpConnection()
{
    if (_buffer != NULL)
    {
        delete[] _buffer;
        _buffer = NULL;
        _bufferSize = 0;
    }

    if (_errorMessage != NULL)
    {
        delete[] _errorMessage;
        _errorMessage = NULL;
    }

    // Just in case ...
    closeSocket();

    return;
} // end of TcpConnection::~TcpConnection()

//---------------------------------------------------------------
// passiveOpen(unsigned short) (Protected)
// Open a server socket.
//
void TcpConnection::passiveOpen(unsigned short port)
{
#ifdef CRTP
    PassiveOpen(port);
#else
    _fsm.PassiveOpen(port);
#endif
    return;
} // end of TcpConnection::passiveOpen(unsigned short)

//---------------------------------------------------------------
// activeOpen(const sockaddr_in&) (Protected)
// Open a client socket.
//
void TcpConnection::activeOpen(const sockaddr_in& address)
{
#ifdef CRTP
    ActiveOpen(&address);
#else
    _fsm.ActiveOpen(&address);
#endif
    return;
} // end of TcpConnection::activeOpen(const sockaddr_in&)

//---------------------------------------------------------------
// acceptOpen(const TcpSegment&) (Protected)
// "Open" an accepted client connection.
//
void TcpConnection::acceptOpen(const TcpSegment& segment)
{
#ifdef CRTP
    AcceptOpen(segment);
#else
    _fsm.AcceptOpen(segment);
#endif
    return;
} // end of TcpConnection::acceptOpen(const TcpSegment&)

//---------------------------------------------------------------
// expandBuffer() (Private)
// Expand the buffer's current size by one block.
//
void TcpConnection::expandBuffer()
{
    char *newBuffer;
    int newSize,
        currBlocks;

    currBlocks = _bufferSize / BUFFER_BLOCK_SIZE;
    newSize = (currBlocks + 1) * BUFFER_BLOCK_SIZE;
    newBuffer = new char[newSize];

    // If the current buffer has data in it, copy it over to the
    // new buffer before deleting the current buffer.
    if (_buffer != NULL)
    {
        (void) memcpy(newBuffer, _buffer, _bufferSize);
        delete[] _buffer;
    }

    _buffer = newBuffer;
    _bufferSize = newSize;

    return;
} // end of TcpConnection::expandBuffer()

#if defined(WIN32)
//---------------------------------------------------------------
// doBind(int) const (Private)
// Bind this UDP socket to a random, available port number.
//
int TcpConnection::doBind(int handle) const
{
    sockaddr_in address;
    unsigned short portIt;
    int socket_error,
        retval;

    // Set up this client's address.
    (void) memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = 0;
    address.sin_addr.s_addr = INADDR_ANY;

    // Start at the largest number and work down.
    for (portIt = MIN_PORT; portIt <= MAX_PORT; ++portIt)
    {
        address.sin_port = htons(portIt);
        if (bind(handle, (sockaddr *) &address, sizeof(sockaddr_in))
                == SOCKET_ERROR)
        {
            socket_error = WSAGetLastError();
            if (socket_error != WSAEADDRINUSE &&
                socket_error != WSAEADDRNOTAVAIL)
            {
                retval = -1;
                break;
            }
        }
        else
        {
            // The bind worked. Return the port number.
            retval = address.sin_port;
            break;
        }
    }

    return(retval);
} // end of TcpConnection::doBind(int) const
#else
//---------------------------------------------------------------
// getLocalPort(int) const (Private)
// Figure out the UDP socket's port number.
//
unsigned short TcpConnection::getLocalPort(int fd) const
{
    socklen_t size;
    sockaddr_in address;
    unsigned short retval;

    size = sizeof(address);
    if (getsockname(fd,
                    reinterpret_cast<sockaddr *>(&address),
                    &size) < 0)
    {
        retval = 0xffff;
    }
    else
    {
        retval = address.sin_port;
    }

    return(retval);
} // end of TcpConnection::getLocalPort(int) const
#endif

//---------------------------------------------------------------
// getLocalAddress() const (Private)
// Return this machine's local internet address. Do this by
// first retrieving the hostname and then doing gethostbyname()
// on itself.
//
unsigned long TcpConnection::getLocalAddress() const
{
    char hostname[MAX_HOSTNAME_LEN];
    hostent *hostEntry;
    unsigned long retval;

    // 1. Get this machine's network host name.
#if defined(__sun)
    (void) sysinfo(SI_HOSTNAME, hostname, MAX_HOSTNAME_LEN);
#else
    (void) gethostname(hostname, MAX_HOSTNAME_LEN);
#endif

    // 2. Get this host name's address.
    if ((hostEntry = gethostbyname(hostname)) != NULL)
    {
        (void) memcpy(&retval,
                      hostEntry->h_addr,
                      hostEntry->h_length);
    }
    else
    {
        // If there is no known address for this host name,
        // default to 127.0.0.1.
        retval = LOCAL_HOST_ADDRESS;
    }

    return(retval);
} // end of TcpConnection::getLocalAddress() const

//---------------------------------------------------------------
// getAck(const TcpSegment&) (Private)
// Figure out what the acknowledgement number should be based on
// the TCP segment's type and size.
//
int TcpConnection::getAck(const TcpSegment& segment)
{
    int retval;

    // The ack # depends on the segment's flags.
    switch (segment.getFlags())
    {
        case TcpSegment::FIN:
        case TcpSegment::SYN:
        case TcpSegment::FIN_ACK:
        case TcpSegment::SYN_ACK:
            retval = segment.getSequenceNumber() + 1;
            break;

        case TcpSegment::PSH:
        case TcpSegment::PSH_ACK:
            retval = segment.getSequenceNumber() +
                     segment.getDataSize();
            break;

        case TcpSegment::ACK:
        default:
            retval = segment.getSequenceNumber();
            break;
    }

    return(retval);
} // end of TcpConnection::getAck(const TcpSegment&)
