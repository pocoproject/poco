//
// Message.cpp
//
// Library: Foundation
// Package: Logging
// Module:  Message
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Message.h"
#include "Poco/Exception.h"
#if !defined(POCO_VXWORKS)
#include "Poco/Process.h"
#endif
#include "Poco/Thread.h"
#include <algorithm>


namespace Poco {


Message::Message():
	_prio(PRIO_FATAL),
	_tid(0),
	_ostid(0),
	_pid(0),
	_file(0),
	_line(0),
	_pMap(0)
{
	init();
}


Message::Message(const std::string& source, const std::string& text, Priority prio):
	_source(source),
	_text(text),
	_prio(prio),
	_tid(0),
	_ostid(0),
	_pid(0),
	_file(0),
	_line(0),
	_pMap(0)
{
	init();
}


Message::Message(const std::string& source, const std::string& text, Priority prio, const char* file, LineNumber line):
	_source(source),
	_text(text),
	_prio(prio),
	_tid(0),
	_ostid(0),
	_pid(0),
	_file(file),
	_line(line),
	_pMap(0)
{
	init();
}


Message::Message(const Message& msg):
	_source(msg._source),
	_text(msg._text),
	_prio(msg._prio),
	_time(msg._time),
	_tid(msg._tid),
	_ostid(msg._ostid),
	_thread(msg._thread),
	_pid(msg._pid),
	_file(msg._file),
	_line(msg._line)
{
	if (msg._pMap)
		_pMap = new StringMap(*msg._pMap);
	else
		_pMap = 0;
}


Message::Message(Message&& msg) noexcept:
	_source(std::move(msg._source)),
	_text(std::move(msg._text)),
	_prio(std::move(msg._prio)),
	_time(std::move(msg._time)),
	_tid(std::move(msg._tid)),
	_ostid(std::move(msg._ostid)),
	_thread(std::move(msg._thread)),
	_pid(std::move(msg._pid)),
	_file(std::move(msg._file)),
	_line(std::move(msg._line))
{
	_pMap = msg._pMap;
	msg._pMap = nullptr;
}


Message::Message(const Message& msg, const std::string& text):
	_source(msg._source),
	_text(text),
	_prio(msg._prio),
	_time(msg._time),
	_tid(msg._tid),
	_ostid(msg._ostid),
	_thread(msg._thread),
	_pid(msg._pid),
	_file(msg._file),
	_line(msg._line)
{
	if (msg._pMap)
		_pMap = new StringMap(*msg._pMap);
	else
		_pMap = 0;
}


Message::~Message()
{
	delete _pMap;
}


void Message::init()
{
#if !defined(POCO_VXWORKS)
	_pid = Process::id();
#endif
	_ostid = (IntPtr)Thread::currentOsTid();
	Thread* pThread = Thread::current();
	if (pThread)
	{
		_tid    = pThread->id();
		_thread = pThread->name();
	}
}


Message& Message::operator = (const Message& msg)
{
	if (&msg != this)
	{
		Message tmp(msg);
		swap(tmp);
	}
	return *this;
}


Message& Message::operator = (Message&& msg) noexcept
{
	_source = std::move(msg._source);
	_text = std::move(msg._text);
	_prio = std::move(msg._prio);
	_time = std::move(msg._time);
	_tid = std::move(msg._tid);
	_ostid = std::move(msg._ostid);
	_thread = std::move(msg._thread);
	_pid = std::move(msg._pid);
	_file = std::move(msg._file);
	_line = std::move(msg._line);
	delete _pMap;
	_pMap = msg._pMap;
	msg._pMap = nullptr;
	return *this;
}


void Message::swap(Message& msg)
{
	using std::swap;
	swap(_source, msg._source);
	swap(_text, msg._text);
	swap(_prio, msg._prio);
	swap(_time, msg._time);
	swap(_tid, msg._tid);
	swap(_ostid, msg._ostid);
	swap(_thread, msg._thread);
	swap(_pid, msg._pid);
	swap(_file, msg._file);
	swap(_line, msg._line);
	swap(_pMap, msg._pMap);
}


void Message::setSource(const std::string& src)
{
	_source = src;
}


void Message::setText(const std::string& text)
{
	_text = text;
}


void Message::setPriority(Priority prio)
{
	_prio = prio;
}


void Message::setTime(const Timestamp& t)
{
	_time = t;
}


void Message::setThread(const std::string& thread)
{
	_thread = thread;
}


void Message::setTid(long tid)
{
	_tid = tid;
}


void Message::setPid(long pid)
{
	_pid = pid;
}


void Message::setSourceFile(const char* file)
{
	_file = file;
}


void Message::setSourceLine(LineNumber line)
{
	_line = line;
}


bool Message::has(const std::string& param) const
{
	return _pMap && (_pMap->find(param) != _pMap->end());
}


const std::string& Message::get(const std::string& param) const
{
	if (_pMap)
	{
		StringMap::const_iterator it = _pMap->find(param);
		if (it != _pMap->end())
	 		return it->second;
	}

	throw NotFoundException();
}


const std::string& Message::get(const std::string& param, const std::string& defaultValue) const
{
	if (_pMap)
	{
		StringMap::const_iterator it = _pMap->find(param);
		if (it != _pMap->end())
	 		return it->second;
	}

	return defaultValue;
}

const Message::StringMap& Message::getAll() const
{
	static StringMap empty;

	if (_pMap)
		return *_pMap;

	return empty;
}

void Message::set(const std::string& param, const std::string& value)
{
	if (!_pMap)
		_pMap = new StringMap;

	std::pair<StringMap::iterator, bool> result =
		_pMap->insert(std::make_pair(param, value));
	if (!result.second)
	{
		result.first->second = value;
	}
}


const std::string& Message::operator [] (const std::string& param) const
{
	if (_pMap)
		return (*_pMap)[param];
	else
		throw NotFoundException();
}


std::string& Message::operator [] (const std::string& param)
{
	if (!_pMap)
		_pMap = new StringMap;
	return (*_pMap)[param];
}


} // namespace Poco
