//
// RefCountedObject.cpp
//
// Library: Foundation
// Package: Core
// Module:  RefCountedObject
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RefCountedObject.h"
#if defined(_DEBUG) && defined(POCO_REFCOUNT_NDC)
#include "Poco/String.h"
#include "Poco/AtomicFlag.h"
#include <iostream>
#endif // _DEBUG && POCO_REFCOUNT_NDC


namespace Poco {


RefCounter::RefCounter() : _counter(1)
{
}


RefCounter::~RefCounter()
{
}


void RefCounter::duplicate() const
{
	++_counter;
}


int RefCounter::release()
{
	int counter = --_counter;
	if (counter == 0)
	{
		delete this;
		return 0;
	}
	poco_assert(counter > 0);
	return counter;
}


int RefCounter::count() const
{
	return _counter;
}


#if defined(_DEBUG) && defined(POCO_REFCOUNT_NDC)


RCDC::TraceRecord::TraceRecord(void *ptr, int refCount, int threshold, const char* func, const std::string &backtrace) : _ptr(ptr),
		_refCount(refCount),
		_threshold(threshold),
		_functionName(func),
		_backtrace(backtrace)
{
}


RCDC::TraceRecord::TraceRecord(const TraceRecord& other): _ptr(other._ptr),
	_refCount(other._refCount),
	_threshold(other._threshold),
	_functionName(other._functionName),
	_backtrace(other._backtrace)
{
}


RCDC::TraceRecord::TraceRecord(TraceRecord&& other): _ptr(other._ptr),
	_refCount(other._refCount),
	_threshold(other._threshold),
	_functionName(other._functionName),
	_backtrace(std::move(other._backtrace))
{
	other._backtrace.clear();
}


RCDC::TraceRecord& RCDC::TraceRecord::operator == (const TraceRecord& other)
{
	Mutex::ScopedLock l(_mutex);
	if (&other != this)
	{
		_ptr = other._ptr;
		_refCount = other._refCount;
		_threshold = other._threshold;
		_functionName = other._functionName;
		_backtrace = other._backtrace;
	}
	return *this;
}


RCDC::TraceRecord& RCDC::TraceRecord::operator == (TraceRecord&& other)
{
	Mutex::ScopedLock l(_mutex);
	if (&other != this)
	{
		_ptr = other._ptr;
		_refCount = other._refCount;
		_threshold = other._threshold;
		_functionName = std::move(other._functionName);
		_backtrace = std::move(other._backtrace);
		other._backtrace.clear();
	}
	return *this;
}


RCDC::TraceMap RCDC::_traceMap;
Mutex          RCDC::_mutex;
RCDC::Ptr      RCDC::_pRCDC(0);
bool           RCDC::_full = false;


RefCountDiagnosticContext::RefCountDiagnosticContext(bool full)
{
	_full = full;
}


RefCountDiagnosticContext::~RefCountDiagnosticContext()
{
	// GH #2261 https://github.com/pocoproject/poco/issues/2261
	dumpLeakRef(std::cerr);
}


void RCDC::dumpRef(std::ostream& os, bool leakOnly) const
{
	typedef OrderedMap<void*, int> SummaryMap;
	AtomicFlag firstStep;
	SummaryMap* pSummary = 0;
	Mutex::ScopedLock l(_mutex);
	for(const auto &entry : traceMap())
	{
		bool out = true;
		const TraceRecord& record = entry.second[entry.second.size() - 1];

		if(leakOnly)
		{
			out = out && entry.second.size() && (record._refCount > record._threshold);
		}

		if(entry.second.size() > 0 && out)
		{
			if(firstStep) os << std::endl;
			if(leakOnly)
			{
				if(!pSummary) pSummary = new SummaryMap;
				(*pSummary)[entry.first] = record._refCount - record._threshold;
				os << "Leaks detected for object at ";
			}
			os << "[0x" << std::hex << entry.first << "]:" << std::endl;
			for(const auto &trace : entry.second)
			{
				std::string nl(1, '\n');
				std::string prevIndent(nl);
				if(trace._refCount > 1)
				{
					for(int i = 0;;)
					{
						prevIndent.append(1, '\t');
						if(++i == trace._refCount) break;
						prevIndent.append(1, '|');
					}
				}
				else prevIndent.append(1, '\t');
				std::string indent;
				std::string nlIndent(nl);
				nlIndent.append(indent);
				os << prevIndent << "refCount=" << trace._refCount << " (" << trace._functionName << ')' << prevIndent << indent << Poco::replace(
						trace._backtrace, nl, prevIndent + indent) << std::endl;
			}
			os << std::endl;
		}
	}
	if (pSummary)
	{
		os << std::endl;
		os << "Leak summary:\n------------" << std::endl;
		for (const auto& s : *pSummary)
			os << s.first << ", refcount=" << s.second << std::endl;
		delete pSummary;
	}
}


void atExitHandler()
{
	delete RCDC::_pRCDC.load();
	RCDC::_pRCDC.store(0);
}


namespace
{
	AtomicFlag beenHere;
}


RCDC* RefCountDiagnosticContext::get()
{
	if (!beenHere)
	{
		if (0 != std::atexit(atExitHandler))
		{
			std::cerr << "Failed to set exit handler, exiting." << std::endl;
			std::exit(EXIT_FAILURE);
		}
		_pRCDC.store(new RCDC);
	}
	return _pRCDC;
}


#endif // _DEBUG && POCO_REFCOUNT_NDC


} // namespace Poco
