//
// StackTrace.h
//
// Library: StackTrace
// Package: StackTrace
// Module:  StackTrace
//
// Definition of the StackTrace class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Trace_StackTrace_INCLUDED
#define Trace_StackTrace_INCLUDED


#include <Poco/Foundation.h>
#include <Poco/Exception.h>
#include <string>
#include <vector>
#include <ostream>


namespace cpptrace {
	struct stacktrace;
}


namespace Poco {
namespace Trace {


class Foundation_API StackTrace
{
public:
	StackTrace(std::size_t skip = 0);
	StackTrace(std::size_t skip, std::size_t maxDepth);
	~StackTrace();

	void print() const;
    void print(std::ostream& stream) const;
    void print(std::ostream& stream, bool color) const;
    void printWithSnippets() const;
    void printWithSnippets(std::ostream& stream) const;
    void printWithSnippets(std::ostream& stream, bool color) const;
    void clear();
    bool empty() const noexcept;
    std::string toString(bool color = false) const;

private:
	std::unique_ptr<cpptrace::stacktrace> pStackTrace;
};


#if 0
using StackTraceFrame = cpptrace::stacktrace_frame;


namespace Poco {
namespace Trace {


class Foundation_API StackTrace
{
public:
	// Represents a single stack frame
	class StackFrame
	{
	public:
		StackFrame(const StackTraceFrame& frame);

		const std::string& getFunction() const;
			/// Returns the function name of this stack frame.

		const std::string& getSource() const;
			/// Returns the source file name of this stack frame.

		int getLine() const;
			/// Returns the line number in the source file of this stack frame.

		int getColumn() const;
			/// Returns the column number in the source file of this stack frame.

	private:
		std::string _function;
		std::string _source;
		unsigned _line;
		unsigned _column;
	};

	static std::vector<StackFrame> captureStackTrace(size_t skip_frames = 0);
		/// Generates a C++ stack trace, skipping the specified number of frames.
		///
		/// Returns a vector of StackFrame objects representing the stack trace.

	static std::vector<StackFrame> captureCStackTrace(size_t skip_frames = 0);
		/// Generates a C stack trace, skipping the specified number of frames.
		///
		/// Returns a vector of StackFrame objects representing the stack trace.

	static void printStackTrace(std::ostream& os, size_t skip_frames = 0);
		/// Generates and prints a C++ stack trace to the specified output stream,
		/// skipping the specified number of frames.

	static void printCStackTrace(std::ostream& os, size_t skip_frames = 0);
		/// Generates and prints a C stack trace to the specified output stream,
		/// skipping the specified number of frames.

	static std::string getCurrentFunctionName();
		/// Returns the demangled name of the current function.

	static std::vector<StackFrame> captureExceptionTrace(const std::exception& e);
		/// Captures a stack trace from the given exception.
		///
		/// Returns a vector of StackFrame objects representing the stack trace.

	static void printExceptionTrace(std::ostream& os, const std::exception& e);
		/// Prints the stack trace of the given exception to the specified output stream.

private:
	StackTrace() = delete;
	StackTrace(const StackTrace&) = delete;
	StackTrace& operator=(const StackTrace&) = delete;
};

#endif
} } // namespace Poco::Trace


#endif // Trace_StackTrace_INCLUDED
