//
// SharedLibrary_UNIX.cpp
//
// $Id: //poco/1.4/Foundation/src/SharedLibrary_UNIX.cpp#3 $
//
// Library: Foundation
// Package: SharedLibrary
// Module:  SharedLibrary
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SharedLibrary_UNIX.h"
#include "Poco/Exception.h"
#include <dlfcn.h>


// Note: cygwin is missing RTLD_LOCAL, set it to 0
#if POCO_OS == POCO_OS_CYGWIN && !defined(RTLD_LOCAL)
#define RTLD_LOCAL 0
#endif


namespace Poco {


FastMutex SharedLibraryImpl::_mutex;


SharedLibraryImpl::SharedLibraryImpl()
{
	_handle = 0;
}


SharedLibraryImpl::~SharedLibraryImpl()
{
}


void SharedLibraryImpl::loadImpl(const std::string& path, int flags)
{
	FastMutex::ScopedLock lock(_mutex);

	if (_handle) throw LibraryAlreadyLoadedException(path);
	int realFlags = RTLD_LAZY;
	if (flags & SHLIB_LOCAL_IMPL)
		realFlags |= RTLD_LOCAL;
	else
		realFlags |= RTLD_GLOBAL;
	_handle = dlopen(path.c_str(), realFlags);
	if (!_handle)
	{
		const char* err = dlerror();
		throw LibraryLoadException(err ? std::string(err) : path);
	}
	_path = path;
}


void SharedLibraryImpl::unloadImpl()
{
	FastMutex::ScopedLock lock(_mutex);

	if (_handle)
	{
		dlclose(_handle);
		_handle = 0;
	}
}


bool SharedLibraryImpl::isLoadedImpl() const
{
	return _handle != 0; 
}


void* SharedLibraryImpl::findSymbolImpl(const std::string& name)
{
	FastMutex::ScopedLock lock(_mutex);

	void* result = 0;
	if (_handle)
	{
		result = dlsym(_handle, name.c_str());
	}
	return result;
}


const std::string& SharedLibraryImpl::getPathImpl() const
{
	return _path;
}


std::string SharedLibraryImpl::prefixImpl()
{
#if POCO_OS == POCO_OS_CYGWIN
	return "cyg";
#else
	return "lib";
#endif
}


std::string SharedLibraryImpl::suffixImpl()
{
#if POCO_OS == POCO_OS_MAC_OS_X
	#if defined(_DEBUG) && !defined(POCO_NO_SHARED_LIBRARY_DEBUG_SUFFIX)
		return "d.dylib";
	#else
		return ".dylib";
	#endif
#elif POCO_OS == POCO_OS_HPUX
	#if defined(_DEBUG) && !defined(POCO_NO_SHARED_LIBRARY_DEBUG_SUFFIX)
		return "d.sl";
	#else
		return ".sl";
	#endif
#elif POCO_OS == POCO_OS_CYGWIN
	#if defined(_DEBUG) && !defined(POCO_NO_SHARED_LIBRARY_DEBUG_SUFFIX)
		return "d.dll";
	#else
		return ".dll";
	#endif
#else
	#if defined(_DEBUG) && !defined(POCO_NO_SHARED_LIBRARY_DEBUG_SUFFIX)
		return "d.so";
	#else
		return ".so";
	#endif
#endif
}


} // namespace Poco
