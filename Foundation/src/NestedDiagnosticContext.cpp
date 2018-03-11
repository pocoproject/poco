//
// NestedDiagnosticContext.cpp
//
// Library: Foundation
// Package: Core
// Module:  NestedDiagnosticContext
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/NestedDiagnosticContext.h"
#include "Poco/SingletonHolder.h"
#include "Poco/String.h"
#include "Poco/ThreadLocal.h"
#include "Poco/Path.h"


namespace Poco {


Mutex NestedDiagnosticContext::_mutex;
NestedDiagnosticContext::TraceMap NestedDiagnosticContext::_traceMap;

NestedDiagnosticContext::NestedDiagnosticContext()
{
}


NestedDiagnosticContext::NestedDiagnosticContext(const NestedDiagnosticContext& ctx)
{
	_stack = ctx._stack;
}


NestedDiagnosticContext::~NestedDiagnosticContext()
{
}


NestedDiagnosticContext& NestedDiagnosticContext::operator = (const NestedDiagnosticContext& ctx)
{
	if (&ctx != this)
		_stack = ctx._stack;
	return *this;
}


void NestedDiagnosticContext::push(const std::string& info)
{
	Context ctx;
	ctx.info = info;
	ctx.line = -1;
	ctx.file = 0;
	_stack.push_back(ctx);
}


void NestedDiagnosticContext::push(const std::string& info, int line, const char* filename)
{
	Context ctx;
	ctx.info = info;
	ctx.line = line;
	ctx.file = filename;
	_stack.push_back(ctx);
}


void NestedDiagnosticContext::pop()
{
	if (!_stack.empty())
		_stack.pop_back();
}


int NestedDiagnosticContext::depth() const
{
	return int(_stack.size());
}


std::string NestedDiagnosticContext::toString() const
{
	std::string result;
	for (Stack::const_iterator it = _stack.begin(); it != _stack.end(); ++it)
	{
		if (!result.empty())
			result.append(":");
		result.append(it->info);
	}
	return result;
}


void NestedDiagnosticContext::dump(std::ostream& ostr) const
{
	dump(ostr, "\n");
}


void NestedDiagnosticContext::dump(std::ostream& ostr, const std::string& delimiter, bool nameOnly) const
{
	for (Stack::const_iterator it = _stack.begin();;)
	{
		std::string file = it->file;
		if (nameOnly) file = Path(file).getFileName();
		ostr << it->info;
		if (it->file)
			ostr << " (in \"" << file << "\", line " << it->line << ")";
		++it;
		if (it == _stack.end()) break;
		ostr << delimiter;
	}
}


void NestedDiagnosticContext::clear()
{
	_stack.clear();
}

std::string NestedDiagnosticContext::backTrace(int skipEnd, int skipBegin, int stackSize, int bufSize)
{
	std::string trace_buf;
#ifdef POCO_COMPILER_GCC
	void* callstack[stackSize];
	const int nMaxFrames = stackSize*sizeof(void*) / sizeof(void*);
	char buf[bufSize];
	int nFrames = backtrace(callstack, nMaxFrames);
	char** symbols = backtrace_symbols(callstack, nFrames);

	for (int i = skipEnd; i < nFrames - skipBegin; ++i)
	{
		Dl_info info;
		if (dladdr(callstack[i], &info) && info.dli_sname)
		{
			char *demangled = NULL;
			int status = 0;
			if (info.dli_sname[0] == '_')
			{
				demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
				if(status == 0)
				{
					snprintf(buf, bufSize, "%-3d %*p %s + %zd\n", i, int(2 + sizeof(void*) * 2), callstack[i],
							status == 0 ? demangled : info.dli_sname == 0 ? symbols[i] : info.dli_sname,
							(char*)callstack[i] - (char*)info.dli_saddr);
				}
				else
				{
					snprintf(buf, bufSize, "__cxa_demangle error: %d", status);
				}
				free(demangled);
			}
		}
		else
		{
			snprintf(buf, bufSize, "%-3d %*p %s\n", i, int(2 + sizeof(void*) * 2), callstack[i], symbols[i]);
		}
		trace_buf.append(buf);
	}
	free(symbols);
	if (nFrames == nMaxFrames)
		trace_buf.append("[truncated]\n");
#else
	trace_buf = "[call trace not available]";
#endif
	return trace_buf;
}


void NestedDiagnosticContext::dumpRef(std::ostream& os, bool leakOnly)
{
	bool first = true;
	for (const auto& entry : _traceMap)
	{
		bool leak = true;
		if (leakOnly) leak = leak && (entry.second[entry.second.size()-1]._refCount >1);
		if (entry.second.size() > 0 && leak)
		{
			if (first)
			{
				os << std::endl;
				first = false;
			}
			os << "0x" << std::hex << entry.first << ':' << std::endl;
			for(const auto &trace : entry.second)
			{
				std::string nl(1, '\n');
				std::string prevIndent(nl);
				if (trace._refCount > 1)
				{
					for (int i=0;;)
					{
						prevIndent.append(1, '\t');
						if (++i == trace._refCount) break;
						prevIndent.append(1, '|');
					}
				}
				else prevIndent.append(1, '\t');
				std::string indent;//(1, '\t');
				std::string nlIndent(nl); nlIndent.append(indent);
				os << prevIndent << indent << "refCount=" << trace._refCount << prevIndent << indent
					<< Poco::replace(trace._backtrace, nl, prevIndent+indent) << std::endl; //<< '|' << prevIndent << indent;
			}
		}
	}
}


namespace
{
	static ThreadLocal<NestedDiagnosticContext> ndc;
}


NestedDiagnosticContext& NestedDiagnosticContext::current()
{
	return ndc.get();
}


} // namespace Poco
