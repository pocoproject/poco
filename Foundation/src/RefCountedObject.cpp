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
#ifdef ENABLE_REFCOUNT_DC
#include "Poco/String.h"
#include "Poco/AtomicFlag.h"
#include <iostream>
#endif // ENABLE_REFCOUNT_DC


namespace Poco {


//
// RefCounter
//

RefCounter::RefCounter(): _counter(1)
{
	poco_rcdc_log;
}


RefCounter::~RefCounter()
{
	poco_rcdc_log;
}


//
// RefCountedObject
//

RefCountedObject::RefCountedObject()
{
}


RefCountedObject::~RefCountedObject()
{
}


//
// WeakRefCounter
//

WeakRefCounter::WeakRefCounter()
{
}


WeakRefCounter::~WeakRefCounter()
{
}


//
// WeakRefCounter
//

WeakRefCountedObject::WeakRefCountedObject(): _pCounter(new WeakRefCounter)
{
}


WeakRefCountedObject::~WeakRefCountedObject()
{
	// prevent leak if release() was never called
	// (eg. this object new-ed into bare pointer
	// and deleted from outside using delete)
	// to prevent double delete, release() flags
	// access by setting the counter pointer to 0
	// before deleting `this`
	delete _pCounter.load();
}


void* WeakRefCounter::operator new(std::size_t)
{
	return getFastMemoryPool<WeakRefCounter>().get();
}


void WeakRefCounter::operator delete(void* ptr)
{
	if (!ptr) return;
	getFastMemoryPool<WeakRefCounter>().release(reinterpret_cast<WeakRefCounter*>(ptr));
}


void* WeakRefCounter::operator new [] (std::size_t)
{
	throw InvalidAccessException("WeakRefCounter arrays not allowed.");
}


void WeakRefCounter::operator delete [] (void* /*ptr*/)
{
}


#ifdef ENABLE_REFCOUNT_DC


RCDC::TraceRecord::TraceRecord(void *ptr, int refCount, const char* func, const std::string &backtrace) : _ptr(ptr),
		_refCount(refCount),
		_functionName(func),
		_backtrace(backtrace)
{
}


RCDC::TraceRecord::TraceRecord(const TraceRecord& other): _ptr(other._ptr),
	_refCount(other._refCount),
	_functionName(other._functionName),
	_backtrace(other._backtrace)
{
}


RCDC::TraceRecord::TraceRecord(TraceRecord&& other): _ptr(other._ptr),
	_refCount(other._refCount),
	_functionName(other._functionName),
	_backtrace(std::move(other._backtrace))
{
	other._backtrace.clear();
}


RCDC::TraceRecord& RCDC::TraceRecord::operator == (const TraceRecord& other)
{
	SpinlockMutex::ScopedLock l(_mutex);
	if (&other != this)
	{
		_ptr = other._ptr;
		_refCount = other._refCount;
		_functionName = other._functionName;
		_backtrace = other._backtrace;
	}
	return *this;
}


RCDC::TraceRecord& RCDC::TraceRecord::operator == (TraceRecord&& other)
{
	SpinlockMutex::ScopedLock l(_mutex);
	if (&other != this)
	{
		_ptr = other._ptr;
		_refCount = other._refCount;
		_functionName = std::move(other._functionName);
		_backtrace = std::move(other._backtrace);
		other._backtrace.clear();
	}
	return *this;
}


RefCountDiagnosticContext::RefCountDiagnosticContext(bool full)
{
	_full = full;
}


RefCountDiagnosticContext::~RefCountDiagnosticContext()
{
	dumpLeakRef(std::cerr);
}


void RCDC::dumpRef(std::ostream& os, bool leakOnly) const
{
	typedef OrderedMap<void*, int> SummaryMap;
	AtomicFlag firstStep;
	SummaryMap* pSummary = 0;
	SpinlockMutex::ScopedLock l(_mutex);
	for(const auto &entry : traceMap())
	{
		bool out = true;
		const TraceRecord& record = entry.second.back();

		if(leakOnly)
		{
			out = out && entry.second.size() && record._refCount > 1;
		}

		if(entry.second.size() > 0 && out)
		{
			if(firstStep) os << std::endl;
			if(leakOnly)
			{
				if(!pSummary) pSummary = new SummaryMap;
				(*pSummary)[entry.first] = record._refCount;
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
				os << prevIndent << "refCount=" << trace._refCount
					<< " (" << trace._functionName << ')'
					<< prevIndent << indent
					<< Poco::replace(trace._backtrace, nl, prevIndent + indent)
					<< std::endl;
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


#endif // ENABLE_REFCOUNT_DC


} // namespace Poco
