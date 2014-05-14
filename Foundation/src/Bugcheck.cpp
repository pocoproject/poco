//
// Bugcheck.cpp
//
// $Id: //poco/1.4/Foundation/src/Bugcheck.cpp#1 $
//
// Library: Foundation
// Package: Core
// Module:  Bugcheck
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Bugcheck.h"
#include "Poco/Debugger.h"
#include "Poco/Exception.h"
#include <sstream>


namespace Poco {


void Bugcheck::assertion(const char* cond, const char* file, int line)
{
	Debugger::enter(std::string("Assertion violation: ") + cond, file, line);
	throw AssertionViolationException(what(cond, file, line));
}


void Bugcheck::nullPointer(const char* ptr, const char* file, int line)
{
	Debugger::enter(std::string("NULL pointer: ") + ptr, file, line);
	throw NullPointerException(what(ptr, file, line));
}


void Bugcheck::bugcheck(const char* file, int line)
{
	Debugger::enter("Bugcheck", file, line);
	throw BugcheckException(what(0, file, line));
}


void Bugcheck::bugcheck(const char* msg, const char* file, int line)
{
	std::string m("Bugcheck");
	if (msg)
	{
		m.append(": ");
		m.append(msg);
	}
	Debugger::enter(m, file, line);
	throw BugcheckException(what(msg, file, line));
}


void Bugcheck::debugger(const char* file, int line)
{
	Debugger::enter(file, line);
}


void Bugcheck::debugger(const char* msg, const char* file, int line)
{
	Debugger::enter(msg, file, line);
}


std::string Bugcheck::what(const char* msg, const char* file, int line)
{
	std::ostringstream str;
	if (msg) str << msg << " ";
	str << "in file \"" << file << "\", line " << line;
	return str.str();
}




} // namespace Poco
