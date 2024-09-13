//
// StackTraceException.cpp
//
// Library: Trace
// Package: Trace
// Module:  StackTraceException
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#if 0
#include "Poco/Trace/StackTraceException.h"
#include <typeinfo>


namespace Poco {
namespace Trace {


StackTraceException::StackTraceException(const std::string& msg):
	Poco::Exception(msg),
	_stackTrace(StackTrace::captureStackTrace(1))
{
}

StackTraceException::StackTraceException(const std::string& msg, const std::string& arg):
	Poco::Exception(msg, arg),
	_stackTrace(StackTrace::captureStackTrace(1))
{
}

StackTraceException::StackTraceException(const std::string& msg, int code):
	Poco::Exception(msg, code),
	_stackTrace(StackTrace::captureStackTrace(1))
{
}

StackTraceException::StackTraceException(const std::string& msg, const std::string& arg, int code):
	Poco::Exception(msg, arg, code),
	_stackTrace(StackTrace::captureStackTrace(1))
{
}

StackTraceException::StackTraceException(const StackTraceException& exc):
	Poco::Exception(exc),
	_stackTrace(exc._stackTrace)
{
}

StackTraceException::~StackTraceException() noexcept
{
}

StackTraceException& StackTraceException::operator = (const StackTraceException& exc)
{
	if (&exc != this)
	{
		Poco::Exception::operator = (exc);
		_stackTrace = exc._stackTrace;
	}
	return *this;
}

const char* StackTraceException::name() const noexcept
{
	return "StackTraceException";
}

const char* StackTraceException::className() const noexcept
{
	return typeid(*this).name();
}

Poco::Exception* StackTraceException::clone() const
{
	return new StackTraceException(*this);
}

void StackTraceException::rethrow() const
{
	throw *this;
}

const std::vector<StackTrace::StackFrame>& StackTraceException::getStackTrace() const
{
	return _stackTrace;
}


} } // namespace Poco::Trace
#endif
