#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H
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
//	Eventloop.h
//
// Description
//  Encapsulates the select-based event loop and timer table.
//
// RCS ID
// $Id: Eventloop.h,v 1.5 2007/12/28 12:34:40 cwrapp Exp $
//
// CHANGE LOG
// $Log: Eventloop.h,v $
// Revision 1.5  2007/12/28 12:34:40  cwrapp
// Version 5.0.1 check-in.
//
// Revision 1.4  2005/05/28 13:31:18  cwrapp
// Updated C++ examples.
//
// Revision 1.0  2003/12/14 19:35:56  charlesr
// Initial revision
//

#include "InputListener.h"
#include "TimerListener.h"
#include <sys/types.h>
#if defined(WIN32)
#include <sys/timeb.h>
#include <winsock2.h>
#else
#include <sys/time.h>
#endif

class Eventloop
{
// Member functions.
public:

    Eventloop();
    ~Eventloop();

    // Enter the event loop and continue running until told to
    // stop. If the event loop terminated normally, 0 is
    // returned; otherwise a return value greater than 0 is
    // returned.
    int start();

    // Stop the event loop.
    void stop();

    // Methods for adding and removing file descriptors.
    void addFD(int fd, InputListener& listener);
    void removeFD(int fd);

    // Methods for starting and stopping timers.
    void startTimer(const char *name,
                    long duration,
                    TimerListener& listener);
    void stopTimer(const char *name, const TimerListener& listener);
    bool doesTimerExist(const char *name) const;

protected:
private:

    void gettime(timeval& timeval) const;

// Inner classes.

    class Timer
    {
    // Member functions.
    public:
        
        Timer(const char *name,
              long duration,
              const timeval& expiration,
              TimerListener& listener);

        ~Timer();

        int operator<(const timeval& time);
        int operator<=(const timeval& time);
        int operator==(const timeval& time);
        int operator>=(const timeval& time);
        int operator>(const timeval& time);

        const char* getName() const;
        long getDuration() const;
        void setDuration(long duration);
        const timeval& getExpiration() const;
        void setExpiration(const timeval& expiration);
        TimerListener& getListener() const;
        Timer* getNext() const;
        void setNext(Timer *timer);

    // Member data.
    private:

        char *_name;

        // Duration is in milliseconds.
        long _duration;

        // When the timer is expected to expire.
        timeval _expiration;

        // The TcpConnection receiving this callback.
        TimerListener& _listener;

        Timer *_next;

    // Friends.
        friend class Eventloop;
    };

// Member data.
public:
protected:
private:

    // Set to true while event loop is running and false when
    // event loop is to stop.
    int _continueRunning;

    // The file descriptors to watch.
    fd_set _fdSet;

    // Keep track of the maximum file descriptor.
    int _maxFD;

    // Keep track of the number of file descriptors being
    // watched.
    int _fdCount;

    // The TCP connection associated with each file descriptor.
    InputListener **_fdTable;

    // This table is sorted by expiration time - from earliest
    // to latest.
    Timer *_timerTable;
};

#endif
