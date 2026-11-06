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
#include "DeferredRelease.h"
#include "Poco/Formatter.h"
#include "Poco/LoggingRegistry.h"
#include "Poco/Exception.h"
#include "Poco/Bugcheck.h"
#include "Poco/NumberFormatter.h"
#include "Poco/NumberParser.h"
#include "Poco/String.h"
#include <new>


namespace Poco {


const std::string Logger::ROOT;
Logger::ExitGuard Logger::_exitGuard;


Logger::ExitGuard::~ExitGuard()
{
	try
	{
		// Not shutdown(): handed to DeferredRelease, an AsyncChannel could be
		// released by its own background thread, which would then wait for itself.
		std::vector<Channel::Ptr> detached;
		{
			Mutex::ScopedLock lock(mapMutex());

			for (auto& [name, pLogger] : loggerMap())
			{
				Channel::Ptr pChannel = pLogger->exchangeChannel(nullptr);
				if (pChannel != nullptr) detached.push_back(std::move(pChannel));
			}
		}
	}
	catch (...)
	{
		poco_unexpected();
	}
}


Logger::Logger(const std::string& name, Channel::Ptr pChannel, int level): _name(name), _pChannel(pChannel), _pCurrentChannel(_pChannel.get()), _level(level)
{
}


Logger::~Logger()
{
}


void Logger::setChannel(Channel::Ptr pChannel)
{
	// The channel that is replaced is let go of when the mutex is free
	// again, since its destructor may log, and not before the threads
	// that log are done with it.
	DeferredRelease::release(exchangeChannel(std::move(pChannel)));
}


Channel::Ptr Logger::exchangeChannel(Channel::Ptr pChannel)
{
	FastMutex::ScopedLock lock(_channelMutex);
	_pChannel.swap(pChannel);
	_pCurrentChannel.store(_pChannel.get());
	return pChannel;
}


Channel::Ptr Logger::getChannel() const
{
	FastMutex::ScopedLock lock(_channelMutex);
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


template <typename M>
void Logger::logImpl(M&& msg)
{
	if (_level >= msg.getPriority())
	{
		// The channel is kept for the time of the reader: a thread that
		// logs takes no mutex and counts no reference.
		DeferredRelease::Reader reader;
		Channel* pChannel = _pCurrentChannel.load();
		if (pChannel)
		{
			pChannel->log(std::forward<M>(msg));
		}
	}
}


void Logger::logToChannel(Message&& msg)
{
	DeferredRelease::Reader reader;
	Channel* pChannel = _pCurrentChannel.load();
	if (pChannel)
	{
		pChannel->log(std::move(msg));
	}
}


void Logger::log(const Message& msg)
{
	logImpl(msg);
}


void Logger::log(Message&& msg)
{
	logImpl(std::move(msg));
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
	if (_level >= prio && hasChannel())
	{
		std::string text(msg);
		formatDump(text, buffer, length);
		logToChannel(Message(_name, text, prio));
	}
}


void Logger::setLevel(const std::string& name, int level)
{
	Mutex::ScopedLock lock(mapMutex());

	std::string::size_type len = name.length();
	for (auto& p: loggerMap())
	{
		if (len == 0 || (p.first.compare(0, len, name) == 0 && (p.first.length() == len || p.first[len] == '.')))
		{
			p.second->setLevel(level);
		}
	}
}


void Logger::setChannel(const std::string& name, Channel::Ptr pChannel)
{
	// The channels that are replaced are let go of when the mutex is free
	// again: a destructor may wait for a thread that asks for a logger.
	std::vector<DeferredRelease::Ptr> replaced;
	{
		Mutex::ScopedLock lock(mapMutex());

		LoggerMap& map = loggerMap();
		replaced.reserve(map.size());
		std::string::size_type len = name.length();
		for (auto& p: map)
		{
			if (len == 0 || (p.first.compare(0, len, name) == 0 && (p.first.length() == len || p.first[len] == '.')))
			{
				replaced.push_back(p.second->exchangeChannel(pChannel));
			}
		}
	}
	DeferredRelease::release(replaced);
}


void Logger::setProperty(const std::string& loggerName, const std::string& propertyName, const std::string& value)
{
	// Channels that are replaced are let go of when the mutex is free again.
	std::vector<DeferredRelease::Ptr> replaced;
	{
		Mutex::ScopedLock lock(mapMutex());

		LoggerMap& map = loggerMap();
		replaced.reserve(map.size());
		std::string::size_type len = loggerName.length();
		for (auto& p: map)
		{
			if (len == 0 || (p.first.compare(0, len, loggerName) == 0 && (p.first.length() == len || p.first[len] == '.')))
			{
				if (propertyName == "channel")
					replaced.push_back(p.second->exchangeChannel(LoggingRegistry::defaultRegistry().channelForName(value)));
				else
					p.second->setProperty(propertyName, value);
			}
		}
	}
	DeferredRelease::release(replaced);
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
			if (it == fmt.end())
			{
				result += '$';
				break;
			}
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
	constexpr int BYTES_PER_LINE = 16;

	message.reserve(message.size() + length*6);
	if (!message.empty()) message.append("\n");
	unsigned char* base = (unsigned char*) buffer;
	std::size_t addr = 0;
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
	Mutex::ScopedLock lock(mapMutex());

	return unsafeGet(name);
}


Logger& Logger::unsafeGet(const std::string& name)
{
	Ptr pLogger = find(name);
	if (!pLogger)
	{
		if (name.empty())
		{
			pLogger = new Logger(name, nullptr, Message::PRIO_INFORMATION);
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
	Mutex::ScopedLock lock(mapMutex());

	if (find(name)) throw ExistsException();
	Ptr pLogger = new Logger(name, pChannel, level);
	add(pLogger);
	return *pLogger;
}


Logger& Logger::root()
{
	Mutex::ScopedLock lock(mapMutex());

	return unsafeGet(std::string());
}


Logger::Ptr Logger::has(const std::string& name)
{
	Mutex::ScopedLock lock(mapMutex());

	return find(name);
}


void Logger::shutdown()
{
	// The channels are let go of when the mutex is free again: a
	// destructor may wait for a thread that asks for a logger.
	std::vector<DeferredRelease::Ptr> detached;
	{
		Mutex::ScopedLock lock(mapMutex());

		// Detach channels from all loggers instead of destroying the logger
		// objects. Singletons and other components may still hold Logger
		// references obtained via Logger::get() during their lifetime.
		// Destroying the loggers would leave those references dangling,
		// causing use-after-free during static destruction.
		LoggerMap& map = loggerMap();
		detached.reserve(map.size());
		for (auto& [name, pLogger] : map)
		{
			if (pLogger != nullptr) detached.push_back(pLogger->exchangeChannel(nullptr));
		}
	}
	DeferredRelease::release(detached);
}


Logger::Ptr Logger::find(const std::string& name)
{
	LoggerMap& map = loggerMap();
	LoggerMap::iterator it = map.find(name);
	if (it != map.end()) return it->second;
	return nullptr;
}


void Logger::destroy(const std::string& name)
{
	// The logger is let go of when the mutex is free again: a destructor
	// may wait for a thread that asks for a logger.
	Ptr pLogger;
	{
		Mutex::ScopedLock lock(mapMutex());

		LoggerMap& map = loggerMap();
		LoggerMap::iterator it = map.find(name);
		if (it != map.end())
		{
			pLogger = std::move(it->second);
			map.erase(it);
		}
	}
}


void Logger::names(std::vector<std::string>& names)
{
	Mutex::ScopedLock lock(mapMutex());

	names.clear();
	for (const auto& p: loggerMap())
	{
		names.push_back(p.first);
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
	else return unsafeGet(std::string());
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
	loggerMap().insert(LoggerMap::value_type(pLogger->name(), pLogger));
}


Mutex& Logger::mapMutex()
{
	// Never destroyed, so that loggers can be used while static objects
	// are constructed and destroyed.
	alignas(Mutex) static unsigned char storage[sizeof(Mutex)];
	static Mutex* const pMutex = ::new (static_cast<void*>(storage)) Mutex;
	return *pMutex;
}


Logger::LoggerMap& Logger::loggerMap()
{
	// Never destroyed, for the same reason as the mutex.
	alignas(LoggerMap) static unsigned char storage[sizeof(LoggerMap)];
	static LoggerMap* const pMap = ::new (static_cast<void*>(storage)) LoggerMap;
	return *pMap;
}


} // namespace Poco
