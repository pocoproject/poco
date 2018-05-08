//
// NestedDiagnosticContext.h
//
// Library: Foundation
// Package: Core
// Module:  NestedDiagnosticContext
//
// Definition of the NestedDiagnosticContext class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//
// g++ backtrace implementation adapted from
// https://panthema.net/2008/0901-stacktrace-demangled/
// License: "You can freely use it for whatever purpose"

#ifndef Foundation_NestedDiagnosticContext_INCLUDED
#define Foundation_NestedDiagnosticContext_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/OrderedMap.h"
#include <vector>
#include <ostream>
#include <typeinfo>
#include <utility>

#if defined(POCO_COMPILER_GCC) && (POCO_OS == POCO_OS_LINUX) && (POCO_OS != POCO_OS_CYGWIN)
	#define POCO_HAS_BACKTRACE
#endif

#ifdef POCO_HAS_BACKTRACE
	#ifdef POCO_COMPILER_GCC
		#include <cxxabi.h>
		#include <execinfo.h>
		#include <dlfcn.h>
	#endif
#endif

namespace Poco {


class NDCScope;


class Foundation_API NestedDiagnosticContext
	/// This class implements a Nested Diagnostic Context (NDC),
	/// as described in Neil Harrison's article "Patterns for Logging
	/// Diagnostic Messages" in "Pattern Languages of Program Design 3"
	/// (Addison-Wesley).
	///
	/// A NDC maintains a stack of context information, consisting of
	/// an informational string (e.g., a method name), as well as an
	/// optional source code line number and file name.
	/// NDCs are especially useful for tagging log messages with
	/// context information which is very helpful in a multithreaded
	/// server scenario.
	/// Every thread has its own private NDC, which is automatically
	/// created when needed and destroyed when the thread terminates.
	///
	/// The NDCScope (or NDC::Scope) class can be used to automatically
	/// push information at the beginning of a scope, and to pop it
	/// at the end.
	/// The poco_ndc(info) macro augments the information with a
	/// source code line number and file name.
{
public:
	typedef NDCScope Scope;

	NestedDiagnosticContext();
		/// Creates the NestedDiagnosticContext.

	NestedDiagnosticContext(const NestedDiagnosticContext& ctx);
		/// Copy constructor.

	~NestedDiagnosticContext();
		/// Destroys the NestedDiagnosticContext.

	NestedDiagnosticContext& operator = (const NestedDiagnosticContext& ctx);
		/// Assignment operator.

	void push(const std::string& info);
		/// Pushes a context (without line number and filename) onto the stack.

	void push(const std::string& info, int line, const char* filename);
		/// Pushes a context (including line number and filename)
		/// onto the stack. Filename must be a static string, such as the
		/// one produced by the __FILE__ preprocessor macro.

	void pop();
		/// Pops the top-most context off the stack.

	int depth() const;
		/// Returns the depth (number of contexts) of the stack.

	std::string toString() const;
		/// Returns the stack as a string with entries
		/// delimited by colons. The string does not contain
		/// line numbers and filenames.

	void dump(std::ostream& ostr) const;
		/// Dumps the stack (including line number and filenames)
		/// to the given stream. The entries are delimited by
		/// a newline.

	void dump(std::ostream& ostr, const std::string& delimiter, bool nameOnly = false) const;
		/// Dumps the stack (including line number and filenames)
		/// to the given stream.
		/// If nameOnly is false (default), the whole path to file is printed,
		/// otherwise only the file name.

	void clear();
		/// Clears the NDC stack.

	static NestedDiagnosticContext& current();
		/// Returns the current thread's NDC.

	template <typename T>
	static std::string typeName(bool full = true)
		/// Returns type name for the provided type.
		/// Names are demangled for g++ only at this time.
		/// If full is false, the scope is trimmed off.
	{
		std::string name(typeid(T).name());
#ifdef POCO_HAS_BACKTRACE
	#ifdef POCO_COMPILER_GCC
		int status = 0;
	#if (POCO_OS == POCO_OS_CYGWIN)
		char* pName = __cxxabiv1::__cxa_demangle(typeid(T).name(), 0, 0, &status);
	#else
		char* pName = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
	#endif
		if (status == 0) name = pName;
		free(pName);
		if (!full) // strip scope, if any
		{
			std::size_t pos = name.rfind("::");
			if (pos != std::string::npos) name = name.substr(pos+2);
		}
	#endif // TODO: demangle other compilers
#endif
		return name;
	}

	static std::string backtrace(int skipEnd = 1, int skipBegin = 0, int stackSize = 128, int bufSize = 1024);
		/// Returns string containing the formatted current thread stack trace. To "trim" returned stack
		/// frames, set skipEnd and skipBegin to the appropriate values; trace entries are sorted in
		/// reverse order (last entry is first). So, to omit this function from the returned string,
		/// skipEnd defaults to 1. For larger stacks, adjust stackSize and bufSize accordingly.
		///
		/// Note that the output may depend on many factors, mostly on the compiler/linker settings,
		/// but also on the code being analyzed in some cases; for example, g++ stack trace will not
		/// show static (or the ones in anonymous namespaces) functions by name, but only by address.

	static bool hasBacktrace();
		/// Returns true if backtrace functionality is available, false otherwise.

private:
	struct Context
	{
		std::string info;
		const char* file;
		int         line;
		std::string trace; // TODO: add backtrace
	};

	typedef std::vector<Context> Stack;

	Stack    _stack;
};


typedef NestedDiagnosticContext NDC;


class Foundation_API NDCScope
	/// This class can be used to automatically push a context onto
	/// the NDC stack at the beginning of a scope, and to pop
	/// the context at the end of the scope.
{
public:
	NDCScope(const std::string& info);
	/// Pushes a context on the stack.

	NDCScope(const std::string& info, int line, const char* filename);
	/// Pushes a context on the stack.

	~NDCScope();
	/// Pops the top-most context off the stack.
};


//
// inlines
//

inline bool NestedDiagnosticContext::hasBacktrace()
{
#ifdef POCO_HAS_BACKTRACE
	return true;
#endif
	return false;
}


inline NDCScope::NDCScope(const std::string& info)
{
	NestedDiagnosticContext::current().push(info);
}


inline NDCScope::NDCScope(const std::string& info, int line, const char* filename)
{
	NestedDiagnosticContext::current().push(info, line, filename);
}


inline NDCScope::~NDCScope()
{
	try
	{
		NestedDiagnosticContext::current().pop();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


//
// helper macros
//
#define poco_ndc_func \
	Poco::NDCScope _theNdcScope(__func__), __LINE__, __FILE__)

#define poco_ndc(func) \
	Poco::NDCScope _theNdcScope(#func, __LINE__, __FILE__)

#define poco_ndc_str(str) \
	Poco::NDCScope _theNdcScope(str, __LINE__, __FILE__)

#define poco_ndc_bt(from, to) \
	Poco::NDCScope _theNdcScope(NDC::backTrace(from, to), __LINE__, __FILE__)


#if defined(_DEBUG)
	#define poco_ndc_func_dbg \
			Poco::NDCScope _theNdcScope(__func__, __LINE__, __FILE__)

	#define poco_ndc_dbg(func) \
			Poco::NDCScope _theNdcScope(#func, __LINE__, __FILE__)

	#define poco_ndc_dbg_str(str) \
			Poco::NDCScope _theNdcScope(str, __LINE__, __FILE__)

	#define poco_ndc_bt_dbg(from, to) \
		Poco::NDCScope _theNdcScope(NDC::backTrace(from, to), __LINE__, __FILE__)
#else
	#define poco_ndc_func_dbg
	#define poco_ndc_dbg(func)
	#define poco_ndc_dbg_str(str)
	#define poco_ndc_bt_dbg(from, to)
#endif


} // namespace Poco


#endif // Foundation_NestedDiagnosticContext_INCLUDED
