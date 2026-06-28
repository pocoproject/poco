//
// Path_WIN32U.h
//
// Library: Foundation
// Package: Filesystem
// Module:  Path
//
// Definition of the PathImpl class for WIN32.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Path_WIN32U_INCLUDED
#define Foundation_Path_WIN32U_INCLUDED


#include "Poco/Foundation.h"
#include <vector>


namespace Poco {


class Foundation_API PathImpl
{
public:
	[[nodiscard]]
	static std::string selfImpl();
	[[nodiscard]]
	static std::string currentImpl();
	[[nodiscard]]
	static std::string homeImpl();
	[[nodiscard]]
	static std::string configHomeImpl();
	[[nodiscard]]
	static std::string dataHomeImpl();
	[[nodiscard]]
	static std::string cacheHomeImpl();
	[[nodiscard]]
	static std::string tempHomeImpl();
	[[nodiscard]]
	static std::string tempImpl();
	[[nodiscard]]
	static std::string configImpl();
	[[nodiscard]]
	static std::string nullImpl();
	[[nodiscard]]
	static std::string systemImpl();
	[[nodiscard]]
	static std::string expandImpl(const std::string& path);
	static void listRootsImpl(std::vector<std::string>& roots);

	enum
	{
		MAX_PATH_LEN = 32767
	};
};


} // namespace Poco


#endif // Foundation_Path_WIN32U_INCLUDED
