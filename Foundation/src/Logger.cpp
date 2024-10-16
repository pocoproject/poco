//
// Logger.cpp
//
// Library: Foundation
// Package: Logging
// Module:  Logger
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Logger.h"
#include "Poco/Formatter.h"
#include "Poco/LoggingRegistry.h"
#include "Poco/Exception.h"
#include "Poco/NumberFormatter.h"
#include "Poco/NumberParser.h"
#include "Poco/String.h"


namespace Poco {


Logger::LoggerMapPtr Logger::_pLoggerMap;
Mutex                Logger::_mapMtx;
const std::string    Logger::ROOT;


Logger::Logger(const std::string& name, Channel::Ptr pChannel, int level): _name(name), _pChannel(pChannel), _level(level)
{
}


Logger::~Logger()
{
}


void Logger::setChannel(Channel::Ptr pChannel)
{
	_pChannel = pChannel;
}


Channel::Ptr Logger::getChannel() const
{
	return _pChannel;
}


void Logger::setLevel(int level)
{
	_level = level;
}


void Logger::setLevel(const std::string& level)
{
	setLevel(parseLevel(level));
}


void Logger::setProperty(const std::string& name, const std::string& value)
{
	if (name == "channel")
		setChannel(LoggingRegistry::defaultRegistry().channelForName(value));
	else if (name == "level")
		setLevel(value);
	else
		Channel::setProperty(name, value);
}


void Logger::log(const Message& msg)
{
	if (_level >= msg.getPriority() && _pChannel)
	{
		_pChannel->log(msg);
	}
}


void Logger::log(const Exception& exc)
{
	error(exc.displayText());
}


void Logger::log(const Exception& exc, const char* file, LineNumber line)
{
	error(exc.displayText(), file, line);
}


void Logger::dump(const std::string& msg, const void* buffer, std::size_t length, Message::Priority prio)
{
	if (_level >= prio && _pChannel)
	{
		std::string text(msg);
		formatDump(text, buffer, length);
		_pChannel->log(Message(_name, text, prio));
	}
}


void Logger::setLevel(const std::string& name, int level)
{
	Mutex::ScopedLock lock(_mapMtx);

	if (_pLoggerMap)
	{
		std::string::size_type len = name.length();
		for (auto& p: *_pLoggerMap)
		{
			if (len == 0 || (p.first.compare(0, len, name) == 0 && (p.first.length() == len || p.first[len] == '.')))
			{
				p.second->setLevel(level);
			}
		}
	}
}


void Logger::setChannel(const std::string& name, Channel::Ptr pChannel)
{
	Mutex::ScopedLock lock(_mapMtx);

	if (_pLoggerMap)
	{
		std::string::size_type len = name.length();
		for (auto& p: *_pLoggerMap)
		{
			if (len == 0 || (p.first.compare(0, len, name) == 0 && (p.first.length() == len || p.first[len] == '.')))
			{
				p.second->setChannel(pChannel);
			}
		}
	}
}


void Logger::setProperty(const std::string& loggerName, const std::string& propertyName, const std::string& value)
{
	Mutex::ScopedLock lock(_mapMtx);

	if (_pLoggerMap)
	{
		std::string::size_type len = loggerName.length();
		for (auto& p: *_pLoggerMap)
		{
			if (len == 0 || (p.first.compare(0, len, loggerName) == 0 && (p.first.length() == len || p.first[len] == '.')))
			{
				p.second->setProperty(propertyName, value);
			}
		}
	}
}


std::string Logger::format(const std::string& fmt, const std::string& arg)
{
	std::string args[] =
	{
		arg
	};
	return format(fmt, 1, args);
}


std::string Logger::format(const std::string& fmt, const std::string& arg0, const std::string& arg1)
{
	std::string args[] =
	{
		arg0,
		arg1
	};
	return format(fmt, 2, args);
}


std::string Logger::format(const std::string& fmt, const std::string& arg0, const std::string& arg1, const std::string& arg2)
{
	std::string args[] =
	{
		arg0,
		arg1,
		arg2
	};
	return format(fmt, 3, args);
}


std::string Logger::format(const std::string& fmt, const std::string& arg0, const std::string& arg1, const std::string& arg2, const std::string& arg3)
{
	std::string args[] =
	{
		arg0,
		arg1,
		arg2,
		arg3
	};
	return format(fmt, 4, args);
}


std::string Logger::format(const std::string& fmt, int argc, std::string argv[])
{
	std::string result;
	std::string::const_iterator it = fmt.begin();
	while (it != fmt.end())
	{
		if (*it == '$')
		{
			++it;
			if (*it == '$')
			{
				result += '$';
			}
			else if (*it >= '0' && *it <= '9')
			{
				int i = *it - '0';
				if (i < argc)
					result += argv[i];
			}
			else
			{
				result += '$';
				result += *it;
			}
		}
		else result += *it;
		++it;
	}
	return result;
}


void Logger::formatDump(std::string& message, const void* buffer, std::size_t length)
{
	const int BYTES_PER_LINE = 16;

	message.reserve(message.size() + length*6);
	if (!message.empty()) message.append("\n");
	unsigned char* base = (unsigned char*) buffer;
	int addr = 0;
	while (addr < length)
	{
		if (addr > 0) message.append("\n");
		message.append(NumberFormatter::formatHex(addr, 4));
		message.append("  ");
		int offset = 0;
		while (addr + offset < length && offset < BYTES_PER_LINE)
		{
			message.append(NumberFormatter::formatHex(base[addr + offset], 2));
			message.append(offset == 7 ? "  " : " ");
			++offset;
		}
		if (offset < 7) message.append(" ");
		while (offset < BYTES_PER_LINE) { message.append("   "); ++offset; }
		message.append(" ");
		offset = 0;
		while (addr + offset < length && offset < BYTES_PER_LINE)
		{
			unsigned char c = base[addr + offset];
			message += (c >= 32 && c < 127) ? (char) c : '.';
			++offset;
		}
		addr += BYTES_PER_LINE;
	}
}


Logger& Logger::get(const std::string& name)
{
	Mutex::ScopedLock lock(_mapMtx);

	return unsafeGet(name);
}


Logger& Logger::unsafeGet(const std::string& name)
{
	Ptr pLogger = find(name);
	if (!pLogger)
	{
		if (name == ROOT)
		{
			pLogger = new Logger(name, 0, Message::PRIO_INFORMATION);
		}
		else
		{
			Logger& par = parent(name);
			pLogger = new Logger(name, par.getChannel(), par.getLevel());
		}
		add(pLogger);
	}
	return *pLogger;
}


Logger& Logger::create(const std::string& name, Channel::Ptr pChannel, int level)
{
	Mutex::ScopedLock lock(_mapMtx);

	if (find(name)) throw ExistsException();
	Ptr pLogger = new Logger(name, pChannel, level);
	add(pLogger);
	return *pLogger;
}


Logger& Logger::root()
{
	Mutex::ScopedLock lock(_mapMtx);

	return unsafeGet(ROOT);
}


Logger::Ptr Logger::has(const std::string& name)
{
	Mutex::ScopedLock lock(_mapMtx);

	return find(name);
}


void Logger::shutdown()
{
	Mutex::ScopedLock lock(_mapMtx);

	_pLoggerMap.reset();
}


Logger::Ptr Logger::find(const std::string& name)
{
	if (_pLoggerMap)
	{
		LoggerMap::iterator it = _pLoggerMap->find(name);
		if (it != _pLoggerMap->end()) return it->second;
	}
	return 0;
}


void Logger::destroy(const std::string& name)
{
	Mutex::ScopedLock lock(_mapMtx);

	if (_pLoggerMap)
	{
		LoggerMap::iterator it = _pLoggerMap->find(name);
		if (it != _pLoggerMap->end()) _pLoggerMap->erase(it);
	}
}


void Logger::names(std::vector<std::string>& names)
{
	Mutex::ScopedLock lock(_mapMtx);

	names.clear();
	if (_pLoggerMap)
	{
		for (const auto& p: *_pLoggerMap)
		{
			names.push_back(p.first);
		}
	}
}


Logger& Logger::parent(const std::string& name)
{
	std::string::size_type pos = name.rfind('.');
	if (pos != std::string::npos)
	{
		std::string pname = name.substr(0, pos);
		Ptr pParent = find(pname);
		if (pParent)
			return *pParent;
		else
			return parent(pname);
	}
	else return unsafeGet(ROOT);
}


int Logger::parseLevel(const std::string& level)
{
	if (icompare(level, "none") == 0)
		return 0;
	else if (icompare(level, "fatal") == 0)
		return Message::PRIO_FATAL;
	else if (icompare(level, "critical") == 0)
		return Message::PRIO_CRITICAL;
	else if (icompare(level, "error") == 0)
		return Message::PRIO_ERROR;
	else if (icompare(level, "warning") == 0)
		return Message::PRIO_WARNING;
	else if (icompare(level, "notice") == 0)
		return Message::PRIO_NOTICE;
	else if (icompare(level, "information") == 0)
		return Message::PRIO_INFORMATION;
	else if (icompare(level, "debug") == 0)
		return Message::PRIO_DEBUG;
	else if (icompare(level, "trace") == 0)
		return Message::PRIO_TRACE;
	else
	{
		int numLevel;
		if (Poco::NumberParser::tryParse(level, numLevel))
		{
			if (numLevel > 0 && numLevel < 9)
				return numLevel;
			else
				throw InvalidArgumentException("Log level out of range ", level);
		}
		else
			throw InvalidArgumentException("Not a valid log level", level);
	}
}


void Logger::add(Ptr pLogger)
{
	if (!_pLoggerMap) _pLoggerMap.reset(new LoggerMap);
	_pLoggerMap->insert(LoggerMap::value_type(pLogger->name(), pLogger));
}


} // namespace Poco
