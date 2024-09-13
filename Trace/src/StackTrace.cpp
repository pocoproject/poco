//
// Array.cpp
//
// Library: Trace
// Package: Trace
// Module:  StackTrace
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "cpptrace/cpptrace.hpp"
#include "Poco/Trace/StackTrace.h"


namespace Poco {
namespace Trace {


StackTrace::StackTrace(std::size_t skip):
	pStackTrace(new cpptrace::stacktrace(cpptrace::generate_trace(skip)))
{
}


StackTrace::StackTrace(std::size_t skip, std::size_t maxDepth):
	pStackTrace(new cpptrace::stacktrace(cpptrace::generate_trace(skip, maxDepth)))
{
}


StackTrace::~StackTrace()
{
}


void StackTrace::print() const
{
	pStackTrace->print();
}


void StackTrace::print(std::ostream& stream) const
{
	pStackTrace->print(stream);
}


void StackTrace::print(std::ostream& stream, bool color) const
{
	pStackTrace->print(stream, color);
}


void StackTrace::printWithSnippets() const
{
	pStackTrace->print_with_snippets();
}


void StackTrace::printWithSnippets(std::ostream& stream) const
{
	pStackTrace->print_with_snippets(stream);
}


void StackTrace::printWithSnippets(std::ostream& stream, bool color) const
{
	pStackTrace->print_with_snippets(stream, color);
}


void StackTrace::clear()
{
	pStackTrace->clear();
}


bool StackTrace::empty() const noexcept
{
	return pStackTrace->empty();
}


std::string StackTrace::toString(bool color) const
{
	return pStackTrace->to_string(color);
}


} } // namespace Poco::Trace
