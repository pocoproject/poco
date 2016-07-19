#ifndef _H_TCPSEGMENT
#define _H_TCPSEGMENT

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
//  TcpSegment.h
//
// Description
//  All TCP transmissions are put into a segment.
//
// RCS ID
// $Id: TcpSegment.h,v 1.6 2007/12/28 12:34:40 cwrapp Exp $
//
// CHANGE LOG
// $Log: TcpSegment.h,v $
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
// Revision 1.0  2003/12/14 19:41:54  charlesr
// Initial revision
//

#if (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#elif defined(WIN32)
#include <iostream>
#include <winsock2.h>
#else
#include <iostream.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

class TcpSegment
{
// Member functions.
public:

    TcpSegment(const sockaddr_in& source_address,
               const sockaddr_in& destination_address,
               unsigned long sequence_number,
               unsigned long ack_number,
               unsigned short flags,
               const char *data,
               int offset,
               int size);

    // Convert the raw bytes into a segment.
    TcpSegment(const sockaddr_in& source_address,
               const sockaddr_in& dest_address,
               const char *data,
               int size);

    // Destructor.
    ~TcpSegment();

    const sockaddr_in& getSource() const;
    const sockaddr_in& getDestination() const;
    unsigned long getSrcAddress() const;
    unsigned short getSrcPort() const;
    unsigned long getDestAddress() const;
    unsigned short getDestPort() const;
    unsigned long getSequenceNumber() const;
    unsigned long getAcknowledgeNumber() const;
    unsigned short getFlags() const;
    int getDataSize() const;
    const char* getData() const;

    // Convert this segment into raw bytes.
    void packetize(char*& data, int& size);

    friend std::ostream& operator<<(std::ostream& stream,
                                    const TcpSegment& segment);

 protected:
 private:

    static char* flagsToString(unsigned short flags);
    static char* addressToString(const sockaddr_in& address);

// Member data.
 public:

    enum TcpHeaderFlags
    {
        NONE= 0x00,
        FIN = 0x01,
        SYN = 0x02,
        RST = 0x04,
        PSH = 0x08,
        ACK = 0x10,
        URG = 0x20,
        FIN_ACK = 0x11,
        SYN_ACK = 0x12,
        RST_ACK = 0x14,
        PSH_ACK = 0x18,
        FLAG_MASK = 0x3f
    };

    static const int TCP_HEADER_SIZE;

 protected:
 private:

    sockaddr_in _src_address;
    sockaddr_in _dest_address;
    unsigned long _sequence_number;
    unsigned long _ack_number;
    unsigned short _flags;
    char *_data;
    int _data_size;
};

#endif
