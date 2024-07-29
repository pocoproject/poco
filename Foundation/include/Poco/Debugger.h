//
// Debugger.h
//
// Library: Foundation
// Package: Core
// Module:  Debugger
//
// Definition of the Debugger class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Debugger_INCLUDED
#define Foundation_Debugger_INCLUDED


#include "Poco/Foundation.h"
#include <string_view>


namespace Poco {


class Foundation_API Debugger
	/// The Debugger class provides an interface to the debugger.
	/// The presence of a debugger can be checked for,
	/// messages can be written to the debugger's log window
	/// and a break into the debugger can be enforced.
	/// The methods only work if the program is compiled
	/// in debug mode (the macro _DEBUG is defined).
{
public:
	static bool isAvailable();
		/// Returns true if a debugger is available, false otherwise.
		/// On Windows, this function uses the IsDebuggerPresent()
		/// function.
		/// On Unix, this function returns true if the environment
		/// variable POCO_ENABLE_DEBUGGER is set.

	static void message(const std::string& msg);
		/// Writes a message to the debugger log, if available, otherwise to
		/// standard error output.

	static void message(const std::string& msg, const char* file, LineNumber line);
		/// Writes a message to the debugger log, if available, otherwise to
		/// standard error output.

	static void enter();
		/// Breaks into the debugger, if it is available.
		/// On Windows, this is done using the DebugBreak() function.
		/// On Unix, the SIGINT signal is raised.

	static void enter(const std::string& msg);
		/// Writes a debug message to the debugger log and breaks into it.

	static void enter(const std::string& msg, const char* file, LineNumber line);
		/// Writes a debug message to the debugger log and breaks into it.

	static void enter(const char* file, LineNumber line);
		/// Writes a debug message to the debugger log and breaks into it.

	static constexpr std::string_view sourceFile(const std::string_view& fileName)
		/// Utility function for reporting the source file name. The file path is
		/// truncated and only the source file name (with extension) is returned.
		///
		/// For full location reporting (including function name and line number),
		/// see `poco_src_loc` macro.
	{
		std::size_t pos = fileName.find_last_of("/\\");
		if (pos == std::string_view::npos) pos = 0;
		else if (fileName.length() > 1) ++pos;
		return std::string_view(fileName.substr(pos));
	}
};


} // namespace Poco


#define poco_src_loc std::string(Poco::Debugger::sourceFile(__FILE__)) \
	.append("::").append(__func__) \
	.append("():").append(std::to_string(__LINE__))


#endif // Foundation_Debugger_INCLUDED
