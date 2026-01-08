//
// SharedLibrary_WIN32U.h
//
// Library: Foundation
// Package: SharedLibrary
// Module:  SharedLibrary
//
// Definition of the SharedLibraryImpl class for Win32.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_SharedLibrary_WIN32U_INCLUDED
#define Foundation_SharedLibrary_WIN32U_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Mutex.h"
#include <vector>
#include <string>


namespace Poco {


class Foundation_API SharedLibraryImpl
{
protected:
	SharedLibraryImpl();
	~SharedLibraryImpl();
	void loadImpl(const std::string& path, int flags);
	void unloadImpl();
	[[nodiscard]]
	bool isLoadedImpl() const;
	[[nodiscard]]
	void* findSymbolImpl(const std::string& name);
	[[nodiscard]]
	const std::string& getPathImpl() const;
	[[nodiscard]]
	static std::string suffixImpl();
	static bool setSearchPathImpl(const std::string& path);
	[[nodiscard]]
	static std::vector<std::string> findMissingDependenciesImpl(const std::string& path);

private:
	std::string _path;
	void* _handle;
	static FastMutex _mutex;
};


} // namespace Poco


#endif // Foundation_SharedLibrary_WIN32U_INCLUDED
