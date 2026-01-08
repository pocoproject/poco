//
// SharedLibrary_VX.h
//
// Library: Foundation
// Package: SharedLibrary
// Module:  SharedLibrary
//
// Definition of the SharedLibraryImpl class for VxWorks.
//
// Copyright (c) 2004-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_SharedLibrary_VX_INCLUDED
#define Foundation_SharedLibrary_VX_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Mutex.h"
#include <moduleLib.h>
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
	MODULE_ID _moduleId;
	static FastMutex _mutex;
};


} // namespace Poco


#endif // Foundation_SharedLibrary_VX_INCLUDED
