//
// Semaphore_STD.h
//
// $Id: //poco/1.4/Foundation/include/Poco/Semaphore_STD.h#1 $
//
// Library: Foundation
// Package: Threading
// Module:  Semaphore
//
// Definition of the SemaphoreImpl class for STD.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Semaphore_STD_INCLUDED
#define Foundation_Semaphore_STD_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include <mutex>
#include <condition_variable>


namespace Poco {


class Foundation_API SemaphoreImpl
{
protected:
	SemaphoreImpl(int n, int max);		
	void setImpl();
	void waitImpl();
	bool waitImpl(long milliseconds);
	
private:
	int _count, _max;
	std::mutex _mutex;
	std::condition_variable _cv;
};


} // namespace Poco


#endif // Foundation_Semaphore_STD_INCLUDED
