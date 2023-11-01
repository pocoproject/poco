//
// SQLChannel.cpp
//
// Library: Data
// Package: Logging
// Module:  SQLChannel
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/SQLChannel.h"
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/BulkBinding.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/LoggingFactory.h"
#include "Poco/Instantiator.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Stopwatch.h"
#include "Poco/Format.h"
#include "Poco/File.h"
#include <fstream>


namespace Poco {
namespace Data {


using namespace Keywords;


const std::string SQLChannel::PROP_CONNECTOR("connector");
const std::string SQLChannel::PROP_CONNECT("connect");
const std::string SQLChannel::PROP_NAME("name");
const std::string SQLChannel::PROP_TABLE("table");
const std::string SQLChannel::PROP_ARCHIVE_TABLE("archive");
const std::string SQLChannel::PROP_MAX_AGE("keep");
const std::string SQLChannel::PROP_ASYNC("async");
const std::string SQLChannel::PROP_TIMEOUT("timeout");
const std::string SQLChannel::PROP_MIN_BATCH("minBatch");
const std::string SQLChannel::PROP_MAX_BATCH("maxBatch");
const std::string SQLChannel::PROP_BULK("bulk");
const std::string SQLChannel::PROP_THROW("throw");
const std::string SQLChannel::PROP_FILE("file");


const std::string SQLChannel::SQL_INSERT_STMT = "INSERT INTO %s " \
	"(Source, Name, ProcessId, Thread, ThreadId, Priority, Text, DateTime)" \
	" VALUES %s";


SQLChannel::SQLChannel():
	_name("-"),
	_table("T_POCO_LOG"),
	_tableChanged(true),
	_timeout(1000),
	_minBatch(DEFAULT_MIN_BATCH_SIZE),
	_maxBatch(DEFAULT_MAX_BATCH_SIZE),
	_bulk(true),
	_throw(false),
	_pid(),
	_tid(),
	_priority(),
	_reconnect(false),
	_running(false),
	_stop(false),
	_logged(0)
{
}


SQLChannel::SQLChannel(const std::string& connector,
	const std::string& connect,
	const std::string& name,
	const std::string& table,
	int timeout,
	int minBatch,
	int maxBatch) :
	_connector(connector),
	_connect(connect),
	_name(name),
	_table(table),
	_tableChanged(true),
	_timeout(timeout),
	_minBatch(minBatch),
	_maxBatch(maxBatch),
	_bulk(false),
	_throw(false),
	_pid(),
	_tid(),
	_priority(),
	_pDBThread(new Thread),
	_reconnect(true),
	_running(false),
	_stop(false),
	_logged(0)
{
	_pDBThread->start(*this);
}


SQLChannel::~SQLChannel()
{
	try
	{
		stop();
		close();
		wait();
		if (_pFileChannel)
			_pFileChannel->close();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


std::string SQLChannel::maskPwd()
{
	std::string displayConnect = _connect;
	Poco::istring is1(displayConnect.c_str());
	Poco::istring is2("pwd=");
	std::size_t pos1 = Poco::isubstr(is1, is2);
	if (pos1 == istring::npos)
	{
		is2 = "password=";
		pos1 = Poco::isubstr(is1, is2);
	}
	if (pos1 != istring::npos)
	{
		pos1 += is2.length();
		std::size_t pos2 = displayConnect.find(';', pos1);
		if (pos2 != std::string::npos)
		{
			std::string toReplace = displayConnect.substr(pos1, pos2-pos1);
			Poco::replaceInPlace(displayConnect, toReplace, std::string("***"));
		}
		else displayConnect.clear();
	}
	return displayConnect;
}


void SQLChannel::open()
{
	if (!_connector.empty() && !_connect.empty())
	{
		try
		{
			_pSession = new Session(_connector, _connect, _timeout / 1000);
			if (_pSession->hasProperty("maxFieldSize")) _pSession->setProperty("maxFieldSize", 8192);
			if (_pSession->hasProperty("autoBind")) _pSession->setFeature("autoBind", true);
			_logger.information("Connected to %s: %s", _connector, maskPwd());
			return;
		}
		catch (DataException& ex)
		{
			_logger.error(ex.displayText());
		}
	}
	_pSession = nullptr;
	return;
}


void SQLChannel::close()
{
	wait(_timeout);
	_pSession = nullptr;
}


void SQLChannel::log(const Message& msg)
{
	_logQueue.enqueueNotification(new LogNotification(msg));
}


size_t SQLChannel::logSync()
{
	try
	{
		return execSQL();
	}
	catch (Exception&)
	{
		if (_throw) throw;
	}

	return 0;
}


bool SQLChannel::processOne(int minBatch)
{
	bool ret = false;
	if (_logQueue.size())
	{
		Notification::Ptr pN = _logQueue.dequeueNotification();
		LogNotification::Ptr pLN = pN.cast<LogNotification>();
		if (pLN)
		{
			const Message& msg = pLN->message();
			_source.push_back(msg.getSource());
			if (_source.back().empty()) _source.back() = _name;
			Poco::replaceInPlace(_source.back(), "'", "''");
			_pid.push_back(msg.getPid());
			_thread.push_back(msg.getThread());
			Poco::replaceInPlace(_thread.back(), "'", "''");
			_tid.push_back(msg.getTid());
			_priority.push_back(msg.getPriority());
			_text.push_back(msg.getText());
			Poco::replaceInPlace(_text.back(), "'", "''");
			_dateTime.push_back(msg.getTime());
		}
		ret = true;
	}
	if (_source.size() >= _minBatch) logSync();

	return ret;
}


void SQLChannel::run()
{
	long sleepTime = 100; // milliseconds
	while (!_stop)
	{
		try
		{
			if (_reconnect)
			{
				close();
				open();
				_reconnect = _pSession.isNull();
				if (_reconnect && sleepTime < 12800)
					sleepTime *= 2;
			}
			processOne(_minBatch);
			sleepTime = 100;
		}
		catch (Poco::Exception& ex)
		{
			_logger.error(ex.displayText());
		}
		catch (std::exception& ex)
		{
			_logger.error(ex.what());
		}
		catch (...)
		{
			_logger.error("SQLChannel::run(): unknown exception");
		}
		_running = true;
		Thread::sleep(100);
	}
	_running = false;
}


void SQLChannel::stop()
{
	if (_pDBThread)
	{
		_reconnect = false;
		_stop = true;
		_pDBThread->join();
		while (_logQueue.size())
			processOne();
	}
}


void SQLChannel::reconnect()
{
	if (!_pDBThread)
	{
		_pDBThread.reset(new Thread);
		_pDBThread->start(*this);
	}
	_reconnect = true;
}


void SQLChannel::setProperty(const std::string& name, const std::string& value)
{
	Poco::FastMutex::ScopedLock l(_mutex);

	if (name == PROP_NAME)
	{
		_name = value;
		if (_name.empty()) _name = "-";
	}
	else if (name == PROP_CONNECTOR)
	{
		_connector = value;
		reconnect();
	}
	else if (name == PROP_CONNECT)
	{
		_connect = value;
		reconnect();
	}
	else if (name == PROP_TABLE)
	{
		_table = value;
		if (_pArchiveStrategy)
			_pArchiveStrategy->setSource(value);
		_tableChanged = true;
	}
	else if (name == PROP_ARCHIVE_TABLE)
	{
		if (value.empty())
		{
			_pArchiveStrategy = 0;
		}
		else if (_pArchiveStrategy)
		{
			_pArchiveStrategy->setDestination(value);
		}
		else
		{
			std::string threshold;
			if (_pArchiveStrategy) threshold = _pArchiveStrategy->getThreshold();
			_pArchiveStrategy = new ArchiveByAgeStrategy(_connector, _connect, _table, value, threshold);
		}
	}
	else if (name == PROP_MAX_AGE)
	{
		if (value.empty() || "forever" == value)
		{
			_pArchiveStrategy = 0;
		}
		else if (_pArchiveStrategy)
		{
			_pArchiveStrategy->setThreshold(value);
		}
		else
		{
			std::string destination = ArchiveByAgeStrategy::DEFAULT_ARCHIVE_DESTINATION;
			if (_pArchiveStrategy) destination = _pArchiveStrategy->getDestination();
			_pArchiveStrategy = new ArchiveByAgeStrategy(_connector, _connect, _table, destination, value);
		}
	}
	else if (name == PROP_ASYNC)
	{
		// no-op
	}
	else if (name == PROP_TIMEOUT)
	{
		if (value.empty() || '0' == value[0])
			_timeout = Statement::WAIT_FOREVER;
		else
			_timeout = NumberParser::parse(value);
	}
	else if (name == PROP_MIN_BATCH)
	{
		int minBatch = NumberParser::parse(value);
		if (!minBatch)
			throw Poco::InvalidArgumentException(Poco::format("SQLChannel::setProperty(%s,%s)", name, value));
		_minBatch = minBatch;
	}
	else if (name == PROP_MAX_BATCH)
	{
		int maxBatch = NumberParser::parse(value);
		if (!maxBatch)
			throw Poco::InvalidArgumentException(Poco::format("SQLChannel::setProperty(%s,%s)", name, value));
		_maxBatch = maxBatch;
	}
	else if (name == PROP_BULK)
	{
		_bulk = isTrue(value);
	}
	else if (name == PROP_THROW)
	{
		_throw = isTrue(value);
	}
	else if (name == PROP_FILE)
	{
		_file = value;
	}
	else
	{
		Channel::setProperty(name, value);
	}
}


std::string SQLChannel::getProperty(const std::string& name) const
{
	Poco::FastMutex::ScopedLock l(_mutex);

	if (name == PROP_NAME)
	{
		if (_name != "-") return _name;
		else return "";
	}
	else if (name == PROP_CONNECTOR)
	{
		return _connector;
	}
	else if (name == PROP_CONNECT)
	{
		return _connect;
	}
	else if (name == PROP_TABLE)
	{
		return _table;
	}
	else if (name == PROP_ARCHIVE_TABLE)
	{
		return _pArchiveStrategy ? _pArchiveStrategy->getDestination() : "";
	}
	else if (name == PROP_MAX_AGE)
	{
		return _pArchiveStrategy ? _pArchiveStrategy->getThreshold() : "forever";
	}
	else if (name == PROP_TIMEOUT)
	{
		return NumberFormatter::format(_timeout);
	}
	else if (name == PROP_MIN_BATCH)
	{
		return std::to_string(_minBatch);
	}
	else if (name == PROP_MAX_BATCH)
	{
		return std::to_string(_maxBatch);
	}
	else if (name == PROP_BULK)
	{
		if (_bulk) return "true";
		else return "false";
	}
	else if (name == PROP_THROW)
	{
		if (_throw) return "true";
		else return "false";
	}
	else if (name == PROP_FILE)
	{
		return _file;
	}
	else
	{
		return Channel::getProperty(name);
	}
}


size_t SQLChannel::logTofile(AutoPtr<FileChannel>& pFileChannel, const std::string& fileName, bool clear)
{
	static std::vector<std::string> names;
	if (names.size() != _source.size())
		names.resize(_source.size(), Poco::replace(_name, "'", "''"));

	std::size_t n = 0;

	if (!pFileChannel) pFileChannel = new FileChannel(fileName);
	if (pFileChannel)
	{
		std::string sql;
		Poco::format(sql, SQL_INSERT_STMT, _table, std::string());
		std::stringstream os;
		os << sql << '\n';
		auto it = _source.begin();
		auto end = _source.end();
		int idx = 0, batch = 0;
		for (; it != end; ++idx)
		{
			std::string dt = Poco::DateTimeFormatter::format(_dateTime[idx], "%Y-%m-%d %H:%M:%S.%i");
			os << "('" << *it << "','" <<
				names[idx] << "'," <<
				_pid[idx] << ",'" <<
				_thread[idx] << "'," <<
				_tid[idx] << ',' <<
				_priority[idx] << ",'" <<
				_text[idx] << "','" <<
				dt << "')";
			if (++batch == _maxBatch)
			{
				os << ";\n";
				Message msg(_source[0], os.str(), Message::PRIO_ERROR);
				pFileChannel->log(msg);
				os.str(""); sql.clear();
				Poco::format(sql, SQL_INSERT_STMT, _table, std::string());
				batch = 0;
			}
			if (++it == end)
			{
				os << ";\n";
				break;
			}
			os << ",\n";
		}
		Message msg(_source[0], os.str(), Message::PRIO_ERROR);
		pFileChannel->log(msg);
		n = _source.size();
		if (clear && n)
		{
			_source.clear();
			_pid.clear();
			_thread.clear();
			_tid.clear();
			_priority.clear();
			_text.clear();
			_dateTime.clear();
		}
	}
	return n;
}


size_t SQLChannel::execSQL()
{
	static std::vector<std::string> names;
	if (names.size() != _source.size())
		names.resize(_source.size(), Poco::replace(_name, "'", "''"));
	static std::string placeholders = "(?,?,?,?,?,?,?,?)";

	Poco::FastMutex::ScopedLock l(_mutex);

	if (_tableChanged)
	{
		Poco::format(_sql, SQL_INSERT_STMT, _table, placeholders);
		_tableChanged = false;
	}

	if (!_pSession || !_pSession->isConnected()) open();
	if (_pArchiveStrategy) _pArchiveStrategy->archive();

	size_t n = 0;
	if (_pSession)
	{
		try
		{
			if (_bulk)
			{
				try
				{
					(*_pSession) << _sql,
						use(_source, bulk),
						use(names, bulk),
						use(_pid, bulk),
						use(_thread, bulk),
						use(_tid, bulk),
						use(_priority, bulk),
						use(_text, bulk),
						use(_dateTime, bulk), now;
				}
				// most likely bulk mode not supported,
				// log and try again
				catch (Poco::InvalidAccessException& ex)
				{
					_logger.log(ex);
					(*_pSession) << _sql,
						use(_source),
						use(names),
						use(_pid),
						use(_thread),
						use(_tid),
						use(_priority),
						use(_text),
						use(_dateTime), now;
					_bulk = false;
				}
			}
			else
			{
				(*_pSession) << _sql,
					use(_source),
					use(names),
					use(_pid),
					use(_thread),
					use(_tid),
					use(_priority),
					use(_text),
					use(_dateTime), now;
			}
			n = _source.size();
		}
		catch (Poco::Exception& ex)
		{
			_logger.error(ex.displayText());
			if (!_file.empty())
				n = logTofile(_pFileChannel, _file);
			close();
			_reconnect = true;
		}
		catch (std::exception& ex)
		{
			_logger.error(ex.what());
			if (!_file.empty())
				n = logTofile(_pFileChannel, _file);
			close();
			_reconnect = true;
		}
	}
	else
	{
		if (!_file.empty())
			n = logTofile(_pFileChannel, _file);
	}
	if (n)
	{
		_logged += n;
		_source.clear();
		_pid.clear();
		_thread.clear();
		_tid.clear();
		_priority.clear();
		_text.clear();
		_dateTime.clear();
	}
	return n;
}


std::size_t SQLChannel::wait(int ms)
{
	Stopwatch sw;
	sw.start();
	int processed = _logQueue.size();
	while (_logQueue.size())
	{
		Thread::sleep(10);
		if (ms && sw.elapsed() * 1000 > ms)
			break;
	}
	return processed - _logQueue.size();
}


void SQLChannel::registerChannel()
{
	Poco::LoggingFactory::defaultFactory().registerChannelClass("SQLChannel",
		new Poco::Instantiator<SQLChannel, Poco::Channel>);
}


} } // namespace Poco::Data
