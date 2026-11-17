//
// InterruptingSignal.h
//
// Definition of the InterruptingSignal class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef InterruptingSignal_INCLUDED
#define InterruptingSignal_INCLUDED


#include "Poco/Foundation.h"


#if defined(POCO_OS_FAMILY_UNIX)


#include <csignal>


extern "C" inline void ignoreSignal(int)
	/// Does nothing; installed as the SIGUSR1 handler, so that the
	/// signal interrupts a blocking system call.
{
}


class InterruptingSignal
	/// Installs a handler for SIGUSR1 that does not restart an
	/// interrupted system call, for the lifetime of the object.
{
public:
	POCO_NODISCARD_CTOR InterruptingSignal();
		/// Installs ignoreSignal() for SIGUSR1 without SA_RESTART
		/// and keeps the previous action.

	~InterruptingSignal();
		/// Restores the previous action for SIGUSR1.

	InterruptingSignal(const InterruptingSignal&) = delete;
	InterruptingSignal& operator = (const InterruptingSignal&) = delete;

private:
	struct sigaction _previous{};
};


//
// inlines
//
inline InterruptingSignal::InterruptingSignal()
{
	struct sigaction action{};
	action.sa_handler = ignoreSignal;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGUSR1, &action, &_previous);
}


inline InterruptingSignal::~InterruptingSignal()
{
	sigaction(SIGUSR1, &_previous, nullptr);
}


#endif // POCO_OS_FAMILY_UNIX


#endif // InterruptingSignal_INCLUDED
