//
// Environment.h
//
// Library: Foundation
// Package: Core
// Module:  Environment
//
// Definition of the Environment class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Environment_INCLUDED
#define Foundation_Environment_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


class Foundation_API Environment
	/// This class provides access to environment variables
	/// and some general system information.
	///
	/// Thread Safety: The get(), has(), and set() methods are
	/// internally synchronized and safe to call concurrently.
	/// However, on POSIX platforms, concurrent use of the C
	/// library functions getenv()/setenv() from other code
	/// (e.g. third-party libraries) is NOT safe. POSIX setenv()
	/// may reallocate environment variable entries, invalidating
	/// pointers returned by a concurrent getenv() in another
	/// thread. Callers that mix Poco::Environment with direct
	/// environment variable access must provide their own
	/// external synchronization.
{
public:
	using NodeId = UInt8[6]; /// Ethernet address.

	[[nodiscard]]
	static std::string get(const std::string& name);
		/// Returns the value of the environment variable
		/// with the given name. Throws a NotFoundException
		/// if the variable does not exist.

	[[nodiscard]]
	static std::string get(const std::string& name, const std::string& defaultValue);
		/// Returns the value of the environment variable
		/// with the given name. If the environment variable
		/// is undefined, returns defaultValue instead.

	[[nodiscard]]
	static bool has(const std::string& name);
		/// Returns true iff an environment variable
		/// with the given name is defined.

	static void set(const std::string& name, const std::string& value);
		/// Sets the environment variable with the given name
		/// to the given value.

	[[nodiscard]]
	static std::string osName();
		/// Returns the operating system name.

	[[nodiscard]]
	static std::string osDisplayName();
		/// Returns the operating system name in a
		/// "user-friendly" way.
		///
		/// Currently this is only implemented for
		/// Windows. There it will return names like
		/// "Windows XP" or "Windows 7/Server 2008 SP2".
		/// On other platforms, returns the same as
		/// osName().

	[[nodiscard]]
	static std::string osVersion();
		/// Returns the operating system version.

	[[nodiscard]]
	static std::string osArchitecture();
		/// Returns the operating system architecture.

	[[nodiscard]]
	static std::string nodeName();
		/// Returns the node (or host) name.

	static void nodeId(NodeId& id);
		/// Returns the Ethernet address of the first Ethernet
		/// adapter found on the system.
		///
		/// Throws a SystemException if no Ethernet adapter is available.

	[[nodiscard]]
	static std::string nodeId();
		/// Returns the Ethernet address (format "xx:xx:xx:xx:xx:xx")
		/// of the first Ethernet adapter found on the system.
		///
		/// Throws a SystemException if no Ethernet adapter is available.

	[[nodiscard]]
	static unsigned int processorCount();
		/// Returns the number of processors installed in the system.
		///
		/// If the number of processors cannot be determined, returns 1.

	[[nodiscard]]
	static Poco::UInt32 libraryVersion();
		/// Returns the POCO C++ Libraries version as a hexadecimal
		/// number in format 0xAABBCCDD, where
		///    - AA is the major version number,
		///    - BB is the minor version number,
		///    - CC is the revision number, and
		///    - DD is the patch level number.
		///
		/// Some patch level ranges have special meanings:
		///    - Dx mark development releases,
		///    - Ax mark alpha releases, and
		///    - Bx mark beta releases.

	[[nodiscard]]
	static Poco::Int32 os();
		/// Return the operating system as defined
		/// in the include Foundation/Platform.h (POCO_OS)

	[[nodiscard]]
	static Poco::Int32 arch();
		/// Return the underlying cpu architecture that runs this operating system
		/// as defined in Foundation/Platform (POCO_ARCH)

	[[nodiscard]]
	static bool isUnix();
		/// Return true if the operating system belongs to the Linux family

	[[nodiscard]]
	static bool isWindows();
		/// Return true if the operating system belongs to the Windows family
};


} // namespace Poco


#endif // Foundation_Environment_INCLUDED
