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
// $Id: Eventloop.cpp,v 1.6 2008/02/04 12:45:07 fperrad Exp $
//
// CHANGE LOG
// $Log: Eventloop.cpp,v $
// Revision 1.6  2008/02/04 12:45:07  fperrad
// fix build on linux (gcc 4.1.3)
//
// Revision 1.5  2007/12/28 12:34:40  cwrapp
// Version 5.0.1 check-in.
//
// Revision 1.4  2005/05/28 13:31:18  cwrapp
// Updated C++ examples.
//
// Revision 1.0  2003/12/14 19:35:40  charlesr
// Initial revision
//

#include "Eventloop.h"
#include <errno.h>
#if !defined(WIN32)
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif

const static long USECS_PER_SEC = 1000000;
const static long USECS_PER_MSEC = 1000;
const static int WIN32_MAX_SOCKET = 1024;

//---------------------------------------------------------------
// Eventloop() (Public)
// Default constructor.
//
Eventloop::Eventloop()
: _continueRunning(1),
  _maxFD(-1),
  _fdCount(0),
  _timerTable(NULL)
{
#if !defined(WIN32)
    rlimit limit;
#endif

    FD_ZERO(&_fdSet);

    // Allocate the file descriptor callback table. First, find
    // out the max file descriptor value.
#if defined(WIN32)
    _fdTable = new InputListener*[WIN32_MAX_SOCKET];
    (void) memset(_fdTable,
                  0,
                  (FD_SETSIZE * sizeof(InputListener*)));
#else
    (void) getrlimit(RLIMIT_NOFILE, &limit);
    _fdTable = new InputListener*[limit.rlim_cur];
    (void) memset(_fdTable,
                  0,
                  (limit.rlim_cur * sizeof(InputListener*)));
#endif

    return;
} // end of Eventloop::Eventloop()

//---------------------------------------------------------------
// ~Eventloop() (Public)
// Destructor.
//
Eventloop::~Eventloop()
{
    Eventloop::Timer *timerIt,
                     *nextTimer;

    // Just in case the event loop is still running.
    _continueRunning = 0;

    if (_fdTable != NULL)
    {
        delete[] _fdTable;
        _fdTable = NULL;
    }

    for (timerIt = _timerTable;
         timerIt != NULL;
         timerIt = nextTimer)
    {
        nextTimer = timerIt->getNext();
        timerIt->setNext(NULL);
        delete timerIt;
    }

    return;
} // end of Eventloop::~Eventloop()

//---------------------------------------------------------------
// start() (Public)
// Start the event loop running.
//
int Eventloop::start()
{
    timeval timeout,
            currTime,
           *timeoutPtr;
    Timer *timer;
    fd_set readSet;
    int i,
        fdCount,
        retcode;
    TimerListener *listener;
    char *timerName;

    _continueRunning = 1;
    retcode = 0;
    while (_continueRunning == 1)
    {
        // Figure out how long to wait. If there are no timers,
        // then set timeoutPtr to NULL and wait forever.
        // Otherwise, put the timeout in the timeval struct and
        // point timeoutPtr to it.
        if (_timerTable == NULL)
        {
            timeoutPtr = NULL;
        }
        else
        {
            // Get the current time and figure out how long until
            // the next timeout.
            gettime(currTime);

            // If we have passed the current timeout value, then
            // don't sleep at all.
            if (*_timerTable < currTime)
            {
                timeout.tv_sec = 0;
                timeout.tv_usec = 0;
            }
            else
            {
                timeout.tv_sec =
                    (_timerTable->getExpiration()).tv_sec - currTime.tv_sec;
                timeout.tv_usec =
                    (_timerTable->getExpiration()).tv_usec - currTime.tv_usec;

                // If the microseconds are less than 0, then subtract
                // a second and add 1,000,000 microseconds.
                if (timeout.tv_usec < 0)
                {
                    --(timeout.tv_sec);
                    timeout.tv_usec += USECS_PER_SEC;
                }
            }

            timeoutPtr = &timeout;
        }

        // Since select modifies the FD sets, pass in a copy.
        FD_ZERO(&readSet);
        for (i = 0; i < _maxFD; ++i)
        {
            if (FD_ISSET(i, &_fdSet))
            {
#if defined(WIN32)
                FD_SET((u_int) i, &readSet);
#else
                FD_SET(i, &readSet);
#endif
            }
        }

        if ((fdCount = select(_maxFD, &readSet, NULL, NULL, timeoutPtr)) < 0)
        {
            // If select() was interrupted, ignore the error.
            if (errno != EINTR)
            {
                _continueRunning = 0;
                retcode = errno;
            }
        }
        else
        {
            // Issue read events first.
            for (i = 0; i < _maxFD && fdCount > 0; ++i)
            {
                if (FD_ISSET(i, &readSet))
                {
                    --fdCount;

                    if (_fdTable[i] != NULL)
                    {
                        _fdTable[i]->handleReceive(i);
                    }
                }
            }

            // Issue timeouts.
            gettime(currTime);
            while (_timerTable != NULL &&
                   *_timerTable <= currTime)
            {
                // Remove the timer from the timer table *before*
                // issuing the callback 'cause the callback may
                // set this same timer again before returning
                // (and the set will fail if the timer is already
                // in the table).
                timer = _timerTable;
                _timerTable = timer->getNext();
                timer->setNext(NULL);

                // Save away the necessary timer info.
                listener = &(timer->getListener());
                timerName = new char[strlen(timer->getName()) + 1];
                strcpy(timerName, timer->getName());

                // Now delete the defunct timer.
                delete timer;

                listener->handleTimeout(timerName);
                delete[] timerName;
            }
        }
    }

    return(retcode);
} // end of Eventloop::start()

//---------------------------------------------------------------
// stop() (Public)
// Stop the event loop.
//
void Eventloop::stop()
{
    _continueRunning = 0;
    return;
} // end of Eventloop::stop()

//---------------------------------------------------------------
// addFD(int, InputListener&) (Public)
// Start watching a file descriptor and route the callback to the
// given connection.
//
void Eventloop::addFD(int fd, InputListener& listener)
{
    if (!FD_ISSET(fd, &_fdSet))
    {
#if defined(WIN32)
        FD_SET((u_int) fd, &_fdSet);
#else
        FD_SET(fd, &_fdSet);
#endif
        ++_fdCount;
    }

    _fdTable[fd] = &listener;

    if (fd >= _maxFD)
    {
        _maxFD = fd + 1;
    }

    return;
} // end of Eventloop::addFD(int, InputListener&)

//---------------------------------------------------------------
// removeFD(int) (Public)
// Stop watching a file descriptor.
//
void Eventloop::removeFD(int fd)
{
    int i;

#if defined(WIN32)
    FD_CLR((u_int) fd, &_fdSet);
#else
    FD_CLR(fd, &_fdSet);
#endif
    _fdTable[fd] = NULL;
    --_fdCount;

    // Are we watching any file descriptors?
    if (_fdCount == 0)
    {
        _maxFD = -1;
    }
    // Are we removing the max fd?
    else if (fd == (_maxFD - 1))
    {
        // Yes. Then look for the next max fd.
        for (i = (fd -1); i >= 0; --i)
        {
            if (FD_ISSET(i, &_fdSet))
            {
                _maxFD = i + 1;
                break;
            }
        }
    }

    return;
} // end of Eventloop::removeFD(int)

//---------------------------------------------------------------
// startTimer(const char*, unsigned long, TimerListener&) (Public)
// Start the named timer.
//
void Eventloop::startTimer(const char *name,
                           long duration,
                           TimerListener& listener)
{
    timeval expiration;
    Eventloop::Timer *timer,
                    **timerIt;

    // Get the current time and add the duration to it.
    gettime(expiration);
    expiration.tv_usec += duration * USECS_PER_MSEC;
    if (expiration.tv_usec >= USECS_PER_SEC)
    {
        expiration.tv_sec += (expiration.tv_usec / USECS_PER_SEC);
        expiration.tv_usec = (expiration.tv_usec % USECS_PER_SEC);
    }

    // Check if this timer already exists. If yes, then remove the
    // timer from the list and update its duration and expiration.
    timer = NULL;
    for (timerIt = &_timerTable;
         *timerIt != NULL;
         timerIt = &((*timerIt)->_next))
    {
        if (strcmp((*timerIt)->getName(), name) == 0 &&
            &((*timerIt)->getListener()) == &listener)
        {
            timer = *timerIt;
            *timerIt = timer->getNext();
            timer->setNext(NULL);

            timer->setDuration(duration);
            timer->setExpiration(expiration);

            break;
        }
    }
    
    if (timer == NULL)
    {
        timer = new Timer(name, duration, expiration, listener);
    }

    // Find out where to put this timer in the list.
    for (timerIt = &_timerTable;
         *timerIt != NULL && **timerIt <= expiration;
         timerIt = &((*timerIt)->_next))
        ;

    timer->setNext(*timerIt);
    *timerIt = timer;

    return;
} // end of Eventloop::startTimer(const char*, long, TimerListener&)

//---------------------------------------------------------------
// stopTimer(const char*, const TimerListener&) (Public)
// Stop the named timer.
//
void Eventloop::stopTimer(const char *name,
                          const TimerListener& listener)
{
    Eventloop::Timer **timerIt,
                      *timer;

    for (timerIt = &_timerTable;
         *timerIt != NULL;
         timerIt = &((*timerIt)->_next))
    {
        if (strcmp((*timerIt)->getName(), name) == 0 &&
            &((*timerIt)->getListener()) == &listener)
        {
            timer = *timerIt;
            *timerIt = timer->getNext();
            timer->setNext(NULL);
            delete timer;

            break;
        }
    }

    return;
} // end of Eventloop::stopTimer(const char*, const TimerListener&)

//---------------------------------------------------------------
// doesTimerExist(const char*) const (Public)
// Return true if the named timer exists.
//
bool Eventloop::doesTimerExist(const char *name) const
{
    Eventloop::Timer **timerIt;
    bool retval;

    for (timerIt = const_cast<Eventloop::Timer**>(&_timerTable),
          retval = false;
         *timerIt != NULL && retval == false;
         timerIt = &((*timerIt)->_next))
    {
        if (strcmp((*timerIt)->getName(), name) == 0)
        {
            retval = true;
        }
    }

    return (retval);
} // end of Eventloop::doesTimerExist(const char*) const

//---------------------------------------------------------------
// Timer(const char*, long, const timeval&, TimerListener&)
// (Public)
// Constructor.
//
Eventloop::Timer::Timer(const char *name,
                        long duration,
                        const timeval& expiration,
                        TimerListener& listener)
: _name(NULL),
  _duration(duration),
  _listener(listener),
  _next(NULL)
{
    _name = new char[strlen(name) + 1];
    (void) strcpy(_name, name);
    (void) memcpy(&_expiration, &expiration, sizeof(_expiration));
} // end of Eventloop::Timer::Timer(...)

//---------------------------------------------------------------
// ~Timer() (Public)
// Destructor.
//
Eventloop::Timer::~Timer()
{
    if (_name != NULL)
    {
        delete[] _name;
        _name = NULL;
    }

    return;
} // end of Eventloop::Timer::~Timer()

//---------------------------------------------------------------
// operator<(const timeval&) (Public)
// Is the expiration time less than the given time?
//
int Eventloop::Timer::operator<(const timeval& time)
{
    return(_expiration.tv_sec < time.tv_sec ||
           (_expiration.tv_sec == time.tv_sec &&
            _expiration.tv_usec < time.tv_usec));
} // end of Eventloop::Timer::operator<(const timeval&)

//---------------------------------------------------------------
// operator<=(const timeval&) (Public)
// Is the expiration time less than or equal to the given time?
//
int Eventloop::Timer::operator<=(const timeval& time)
{
    return(_expiration.tv_sec < time.tv_sec ||
           (_expiration.tv_sec == time.tv_sec &&
            _expiration.tv_usec <= time.tv_usec));
} // end of Eventloop::Timer::operator<=(const timeval&)

//---------------------------------------------------------------
// operator==(const timeval&) (Public)
// Is the expiration time equal to the given time?
//
int Eventloop::Timer::operator==(const timeval& time)
{
    return(_expiration.tv_sec == time.tv_sec &&
            _expiration.tv_usec == time.tv_usec);
} // end of Eventloop::Timer::operator==(const timeval&)

//---------------------------------------------------------------
// operator>=(const timeval&) (Public)
// Is the expiration time greater than or equal to the given
// time?
//
int Eventloop::Timer::operator>=(const timeval& time)
{
    return(_expiration.tv_sec > time.tv_sec ||
           (_expiration.tv_sec == time.tv_sec &&
            _expiration.tv_usec >= time.tv_usec));
} // end of Eventloop::Timer::operator>=(const timeval&)

//---------------------------------------------------------------
// operator>(const timeval&) (Public)
// Is the expiration time greater than the given time?
//
int Eventloop::Timer::operator>(const timeval& time)
{
    return(_expiration.tv_sec > time.tv_sec ||
           (_expiration.tv_sec == time.tv_sec &&
            _expiration.tv_usec > time.tv_usec));
} // end of Eventloop::Timer::operator>(const timeval&)

//---------------------------------------------------------------
// getName() const (Public)
// Return the timer's name.
//
const char* Eventloop::Timer::getName() const
{
    return(_name);
} // end of Eventloop::Timer::getName() const

//---------------------------------------------------------------
// getDuration() const (Public)
// Return the timer's duration in milliseconds.
//
long Eventloop::Timer::getDuration() const
{
    return(_duration);
} // end of Eventloop::Timer::getDuration() const

//---------------------------------------------------------------
// setDuration(long) (Public)
// Set the timer's duration.
//
void Eventloop::Timer::setDuration(long duration)
{
    _duration = duration;
    return;
} // end of Eventloop::Timer::setDuration(long)

//---------------------------------------------------------------
// getExpiration() const (Public)
// Return the timer's expiration.
//
const timeval& Eventloop::Timer::getExpiration() const
{
    return(_expiration);
} // end of Eventloop::Timer::getExpiration() const

//---------------------------------------------------------------
// setExpiration(const timeval&) (Public)
// Set the timer's expiration.
//
void Eventloop::Timer::setExpiration(const timeval& expiration)
{
    (void) memcpy(&_expiration, &expiration, sizeof(_expiration));
    return;
} // end of Eventloop::Timer::setExpiration(const timeval&)

//---------------------------------------------------------------
// getListener() const (Public)
// Return timer's callback connection object.
//
TimerListener& Eventloop::Timer::getListener() const
{
    return(_listener);
} // end of Eventloop::Timer::getListener() const

//---------------------------------------------------------------
// getNext() const (Public)
// Return the next pointer.
//
Eventloop::Timer* Eventloop::Timer::getNext() const
{
    return(_next);
} // end of Eventloop::Timer::getNext() const

//---------------------------------------------------------------
// setNext(Timer*) (Public)
// Set the next pointer.
//
void Eventloop::Timer::setNext(Eventloop::Timer *timer)
{
    _next = timer;
    return;
} // end of Eventloop::Timer::setNext(Eventloop::Timer*)

//---------------------------------------------------------------
// gettime(timeval&) const (Private)
// Get the current time.
//
void Eventloop::gettime(timeval& timeval) const
{
#if defined(WIN32)
    _timeb currTime;

    _ftime(&currTime);
    timeval.tv_sec = currTime.time;
    timeval.tv_usec = currTime.millitm * USECS_PER_MSEC;
#else
    (void) gettimeofday(&timeval, NULL);
#endif

    return;
} // end of Eventloop::gettime(timeval&) const
