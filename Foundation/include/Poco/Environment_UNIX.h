//
// Environment_UNIX.h
//
// Library: Foundation
// Package: Core
// Module:  Environment
//
// Definition of the EnvironmentImpl class for Unix.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Environment_UNIX_INCLUDED
#define Foundation_Environment_UNIX_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Mutex.h"
#include <map>


namespace Poco {


class Foundation_API EnvironmentImpl
{
public:
	using NodeId = UInt8[6]; /// Ethernet address.

	[[nodiscard]]
	static std::string getImpl(const std::string& name);
	[[nodiscard]]
	static bool hasImpl(const std::string& name);
	static void setImpl(const std::string& name, const std::string& value);
	[[nodiscard]]
	static std::string osNameImpl();
	[[nodiscard]]
	static std::string osDisplayNameImpl();
	[[nodiscard]]
	static std::string osVersionImpl();
	[[nodiscard]]
	static std::string osArchitectureImpl();
	[[nodiscard]]
	static std::string nodeNameImpl();
	static void nodeIdImpl(NodeId& id);
	[[nodiscard]]
	static unsigned int processorCountImpl();

private:
	static FastMutex _mutex;
};


} // namespace Poco


#endif // Foundation_Environment_UNIX_INCLUDED
