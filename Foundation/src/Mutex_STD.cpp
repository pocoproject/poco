//
// Mutex_WIN32.cpp
//
// $Id: //poco/1.4/Foundation/src/Mutex_STD.cpp#1 $
//
// Library: Foundation
// Package: Threading
// Module:  Mutex
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Mutex_STD.h"
#include "Poco/Timestamp.h"


namespace Poco {


MutexImpl::MutexImpl(MutexTypeImpl type)
	: _mutex(type == MUTEX_RECURSIVE_IMPL ? 
		std::unique_ptr<MutexImpl_BaseMutex>(new MutexImpl_MutexI<std::recursive_timed_mutex>()) : 
		std::unique_ptr<MutexImpl_BaseMutex>(new MutexImpl_MutexI<std::timed_mutex>()))
{
}


FastMutexImpl::FastMutexImpl() : _mutex()
{
}


} // namespace Poco
