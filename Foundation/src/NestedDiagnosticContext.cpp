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
		if (!it->trace.empty())
			ostr << "\nbacktrace:" << it->trace;
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
	std::string traceBuf;
#ifdef POCO_COMPILER_GCC
	const int maxFrames = sizeof(void*) * stackSize;
	void* addrList[maxFrames+1];
	int addrLen = backtrace(addrList, maxFrames);

	if (addrLen != 0)
	{
		char** symbollist = backtrace_symbols(addrList, addrLen);

		size_t funcNameSize = 256;
		char* funcName = (char*)malloc(funcNameSize);

		int begin = skipEnd;
		int end = addrLen - ((skipBegin <= addrLen) ? skipBegin : 0);
		traceBuf.append("backtrace");
		if (begin > 0)
		{
			traceBuf.append(" (entries 1-" + std::to_string(begin-1) + " skipped)");
		}
		traceBuf.append(":\n");

		std::string prevSymbol;
		for (int i = begin; i < end; ++i)
		{
			char *beginName = 0, *beginOffset = 0, *endOffset = 0;

			// find parentheses and +address offset surrounding the mangled name:
			// ./module(function+0x15c) [0x8048a6d]
			for (char*p = symbollist[i]; *p; ++p)
			{
				if (*p == '(') beginName = p;
				else
				{
					if (*p == '+') beginOffset = p;
					else if (*p == ')' && beginOffset)
					{
						endOffset = p;
						break;
					}
				}
			}

			if (beginName && beginOffset && endOffset && beginName < beginOffset)
			{
				*beginName++ = '\0';
				*beginOffset++ = '\0';
				*endOffset = '\0';

				// mangled name:  [beginName, beginOffset),
				// caller offset: [beginOffset, endOffset)
				int status;
				char* ret = abi::__cxa_demangle(beginName, funcName, &funcNameSize, &status);
				if (status == 0) // demangled
				{
					funcName = ret;
					if (prevSymbol != symbollist[i]) // output module once
					{
						traceBuf.append("\n> ");
						traceBuf.append(symbollist[i]);
						traceBuf.append(":\n");
						prevSymbol = symbollist[i];
					}
					traceBuf.append(1, ' ').append(std::to_string(i)).append(1, ' ');
					traceBuf.append(funcName).append(1, '+').append(beginOffset);
					traceBuf.append(1, '\n');
				}
				else // demangling failed, output mangled
				{
					traceBuf.append("\n> ");
					traceBuf.append(symbollist[i]);
					traceBuf.append(":\n");
					traceBuf.append(1, ' ').append(std::to_string(i)).append(1, ' ');
					traceBuf.append(beginName).append(1, '+').append(beginOffset);
					traceBuf.append(1, '\n');
				}
			}
			else // couldn't parse, output everything
			{
				traceBuf.append(symbollist[i]).append(1, '\n');
			}
		}
		if (traceBuf.size()) traceBuf.erase(traceBuf.end()-1);
		free(funcName);
		free(symbollist);
	}
#else
	traceBuf = "[call trace not available]";
#endif
	return traceBuf;
}

/*
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
				std::string indent;
				std::string nlIndent(nl); nlIndent.append(indent);
				os << prevIndent << indent << trace._entry << ", refCount=" << trace._refCount << prevIndent << indent
						<< Poco::replace(trace._backtrace, nl, prevIndent+indent) << std::endl;
			}
		}
	}
}
*/

namespace
{
	static ThreadLocal<NestedDiagnosticContext> ndc;
}


NestedDiagnosticContext& NestedDiagnosticContext::current()
{
	return ndc.get();
}


} // namespace Poco
