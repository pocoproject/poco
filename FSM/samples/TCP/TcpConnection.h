#ifndef _H_TCPCONNECTION
#define _H_TCPCONNECTION

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
//  TcpConnection.h
//
// Description
//  TCP connection class header.
//
// RCS ID
// $Id: TcpConnection.h,v 1.6 2015/08/02 19:44:35 cwrapp Exp $
//
// CHANGE LOG
// $Log: TcpConnection.h,v $
// Revision 1.6  2015/08/02 19:44:35  cwrapp
// Release 6.6.0 commit.
//
// Revision 1.5  2007/12/28 12:34:40  cwrapp
// Version 5.0.1 check-in.
//
// Revision 1.4  2005/05/28 13:31:18  cwrapp
// Updated C++ examples.
//
// Revision 1.0  2003/12/14 19:40:45  charlesr
// Initial revision
//

#include <sys/types.h>
#if defined(WIN32)
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif
#include "TcpSegment.h"
#include "TcpConnection_sm.h"
#include "TcpConnectionListener.h"
#include "InputListener.h"
#include "TimerListener.h"

class TcpConnection :
    /* implements */ public InputListener,
    /* implements */ public TimerListener
#ifdef CRTP
    /* extends */ , public TcpConnectionContext<TcpConnection>
#endif
{
// Member functions.
 public:

    virtual ~TcpConnection();

    // Return the far-end's 4-byte IP address.
    unsigned long getFarAddress() const;

    // Return the far-end's TCP port.
    unsigned short getFarPort() const;

    // Return the current sequence number.
    unsigned long getSequenceNumber() const;

    // Send the raw bytes to the far end client socket.
    void transmit(const char *data,
                  int offset,
                  int size);

    // Start closing this connection.
    void doClose();

    // Set an accepted client socket's listener.
    void setListener(TcpConnectionListener& listener);

    // Socket event handling methods.
    void handleReceive(int fd);
    void handleTimeout(const char *name);

    //-----------------------------------------------------------
    // State Machine Actions
    //
    void openServerSocket(unsigned short port);
    void openClientSocket(const sockaddr_in *address);
    void openSuccess();
    void openFailed(const char *reason);
    void closeSocket();
    void halfClosed();
    void closed(const char *reason);
    void clearListener();
    void transmitted();
    void transmitFailed(const char *reason);
    void receive(const TcpSegment& segment);
    void sendOpenSyn(const sockaddr_in *address);
    void accept(const TcpSegment& segment);
    void accepted();
    void sendSynAck(const TcpSegment& segment);
    void sendSynAckAck(const TcpSegment& segment);
    void doSend(unsigned short flags,
                const char *data,
                int offset,
                int size,
                const TcpSegment *recv_segment);
    void startTimer(const char *name, time_t time);
    void stopTimer(const char *name);
    void setNearAddress();
    void setFarAddress(const TcpSegment& segment);
    void deleteSegment(const TcpSegment& segment);

 protected:

    // Server and active client socket constructor.
    TcpConnection(TcpConnectionListener& listener);

    // "Accepted" socket constructor.
    TcpConnection(const sockaddr_in& near_address,
                  const sockaddr_in& far_address,
#if defined(WIN32)
                  unsigned short actual_port,
                  SOCKET udp_socket,
                  HANDLE udp_handle,
#else
                  int udp_socket,
#endif
                  int sequence_number,
                  TcpServer& server,
                  TcpConnectionListener& listener);

    // Open a server socket.
    void passiveOpen(unsigned short port);

    // Open a client socket.
    void activeOpen(const sockaddr_in& address);

    // An accepted client socket.
    void acceptOpen(const TcpSegment& segment);
                  
 private:

    // Copy construction and assignment not allowed.
    TcpConnection(const TcpConnection&);
    const TcpConnection& operator=(const TcpConnection&);

    // Increase the buffer's size.
    void expandBuffer();

#if defined(WIN32)
    // Windows doesn't bind UDP sockets to random ports
    // correctly. So this app has to do it itself.
    int doBind(int handle) const;
#else
    // Return the UDP socket's port.
    unsigned short getLocalPort(int fd) const;
#endif

    // Return this machine's address.
    unsigned long getLocalAddress() const;

    // Return the expected ack number for the given segment.
    int getAck(const TcpSegment& segment);

// Member data.
 public:

    // Constants.
    // Initial Sequence Number.
    const static unsigned long ISN;

    // Minimum timeout is 1 millisecond.
    const static int MIN_TIMEOUT;

    // Wait only so long for an ACK (in milliseconds).
    const static int ACK_TIMEOUT;

    // Wait a while before reusing this port (in milliseconds).
    const static int CLOSE_TIMEOUT;

    // Allocate the input buffer using this block size.
    const static int BUFFER_BLOCK_SIZE;

 protected:

    // Send TCP events to this listener.
    TcpConnectionListener *_listener;

    // Connected to this address.
    sockaddr_in _farAddress;

    // This socket's address.
    sockaddr_in _nearAddress;

#if defined(WIN32)
    // This socket's actual port number.
    unsigned short _actualPort;
#endif

 private:

#if defined(WIN32)
    SOCKET _udp_win_socket;
#endif
    int _udp_socket;
    unsigned long _sequence_number;

    // Read data into the following buffer.
    char *_buffer;
    int _bufferSize;

    // The server which accepted this connection. Will only be
    // set if this is an "accepted" client connection.
    TcpServer *_server;

    // Store failure messages here.
    char *_errorMessage;

#ifndef CRTP
    TcpConnectionContext _fsm;
#endif
};

#endif
