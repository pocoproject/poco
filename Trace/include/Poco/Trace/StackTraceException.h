//
// JSONException.h
//
// Library: Trace
// Package: StackTrace
// Module:  StackTraceException
//
// Definition of the StackTraceException class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#if 0

#ifndef Trace_StackTraceException_INCLUDED
#define Trace_StackTraceException_INCLUDED


#include "Poco/Trace/StackTrace.h"
#include "Poco/Exception.h"


namespace Poco {
namespace Trace {


//POCO_DECLARE_EXCEPTION(JSON_API, JSONException, Poco::Exception)

// Exception class that includes a stack trace
class Foundation_API StackTraceException : public Poco::Exception
{
public:
	StackTraceException(const std::string& msg);
		/// Creates a StackTraceException with the given message and captures a stack trace.

	StackTraceException(const std::string& msg, const std::string& arg);
		/// Creates a StackTraceException with the given message and argument, and captures a stack trace.

	StackTraceException(const std::string& msg, int code);
		/// Creates a StackTraceException with the given message and error code, and captures a stack trace.

	StackTraceException(const std::string& msg, const std::string& arg, int code);
		/// Creates a StackTraceException with the given message, argument, and error code, and captures a stack trace.

	StackTraceException(const StackTraceException& exc);
		/// Copy constructor.

	~StackTraceException() noexcept;
		/// Destroys the StackTraceException.

	StackTraceException& operator = (const StackTraceException& exc);
		/// Assignment operator.

	const char* name() const noexcept override;
		/// Returns the name of the exception.

	const char* className() const noexcept override;
		/// Returns the name of the exception class.

	Poco::Exception* clone() const override;
		/// Creates a clone of the exception.

	void rethrow() const override;
		/// (Re)throws the exception.

	const std::vector<StackTrace::StackFrame>& getStackTrace() const;
		/// Returns the captured stack trace.

private:
	std::vector<StackTrace::StackFrame> _stackTrace;
};


} } // namespace Poco::Trace


#endif // Trace_StackTraceException_INCLUDED
#endif