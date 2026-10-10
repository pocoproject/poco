//
// NamedEvent_UNIX.h
//
// Library: Foundation
// Package: Processes
// Module:  NamedEvent
//
// Definition of the NamedEventImpl class for Unix.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_NamedEvent_UNIX_INCLUDED
#define Foundation_NamedEvent_UNIX_INCLUDED

#include "Poco/Foundation.h"

#include <Poco/Platform.h>

#if defined(sun) || defined(__APPLE__) || defined(__osf__) || defined(__QNX__) || defined(_AIX) || defined(__GNU__)
	#define POCO_NAMED_EVENT_USE_POSIX_SEMAPHORES 1
	#define POCO_NAMED_EVENT_USE_SYS_V_SEMAPHORES 0
#else
	#define POCO_NAMED_EVENT_USE_POSIX_SEMAPHORES 0
	#define POCO_NAMED_EVENT_USE_SYS_V_SEMAPHORES 1
#endif

#if POCO_NAMED_EVENT_USE_POSIX_SEMAPHORES
	#include <semaphore.h>
#endif

namespace Poco {


class Foundation_API NamedEventImpl
{
protected:
	NamedEventImpl(const std::string& name);
	~NamedEventImpl();
	void setImpl();
	void waitImpl();

private:
	std::string getFileName();

	std::string _name;
#if POCO_NAMED_EVENT_USE_POSIX_SEMAPHORES
	::sem_t* _sem;
#else
	int _semid;  // semaphore id
	bool _createdId; // semaphore id was created with this instance
#endif
};


} // namespace Poco


#endif // Foundation_NamedEvent_UNIX_INCLUDED
