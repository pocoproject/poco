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
#include "Poco/Data/BulkBinding.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/LoggingFactory.h"
#include "Poco/Instantiator.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Format.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/UUID.h"
#include "Poco/UUIDGenerator.h"

#include <memory>


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
const std::string SQLChannel::PROP_MAX_SQL("maxSQL");
const std::string SQLChannel::PROP_BULK("bulk");
const std::string SQLChannel::PROP_THROW("throw");
const std::string SQLChannel::PROP_DIRECTORY("directory");
const std::string SQLChannel::PROP_FILE("file");
const std::string SQLChannel::PROP_FLUSH("flush");


const std::string SQLChannel::SQL_INSERT_STMT = "INSERT INTO %s " \
	"(Source, Name, ProcessId, Thread, ThreadId, Priority, Text, DateTime)" \
	" VALUES %s";


SQLChannel::SQLChannel():
	_name("SQLChannel"),
	_tableChanged(true),
	_timeout(1000),
	_minBatch(DEFAULT_MIN_BATCH_SIZE),
	_maxBatch(DEFAULT_MAX_BATCH_SIZE),
	_maxSQL(DEFAULT_MAX_SQL_SIZE),
	_bulk(true),
	_throw(false),
	_pid(),
	_tid(),
	_priority(),
	_pLogThread(new Thread),
	_reconnect(false),
	_stop(false),
	_logged(0)
{
	_pLogThread->start(*this);
}


SQLChannel::SQLChannel(const std::string& connector,
	const std::string& connect,
	const std::string& name,
	const std::string& table,
	int timeout,
	int minBatch,
	int maxBatch,
	int maxSQL) :
	_connector(connector),
	_connect(connect),
	_name(name),
	_table(table),
	_tableChanged(true),
	_timeout(timeout),
	_minBatch(minBatch),
	_maxBatch(maxBatch),
	_maxSQL(maxSQL),
	_bulk(false),
	_throw(false),
	_pid(),
	_tid(),
	_priority(),
	_pLogThread(new Thread),
	_reconnect(true),
	_stop(false),
	_logged(0)
{
	_pLogThread->start(*this);
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
			if (!_stop) _logger.information("Connected to %s: %s", _connector, maskPwd());
			else logLocal(Poco::format("Connected to %s: %s", _connector, maskPwd()));
			return;
		}
		catch (const DataException& ex)
		{
			logLocal(ex.displayText());
		}
	}
	_pSession = nullptr;
	return;
}


void SQLChannel::close()
{
	wait(_timeout);
	if (_pSession)
	{
		_pSession->close();
		_pSession = nullptr;
	}
}


void SQLChannel::logLocal(const std::string& message, Message::Priority prio)
{
	Message msg("SQLChannel"s, message, prio);
	log(msg);
}


void SQLChannel::log(const Message& msg)
{
	_logQueue.enqueueNotification(new LogNotification(msg));
	_event.set();
}


size_t SQLChannel::logSync(bool flush)
{
	try
	{
		return execSQL(flush);
	}
	catch (...)
	{
		if (_throw) throw;
	}

	return 0;
}


bool SQLChannel::processBatch(int minBatch)
{
	bool ret = false, flush = (minBatch == 0);
	while (_logQueue.size())
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
			_dateTime.emplace_back(msg.getTime());
			if ((_source.size() >= minBatch) || flush)
				logSync(flush);
			ret = true;
		}
	}
	if (flush) logSync(flush);

	return ret;
}


void SQLChannel::run()
{
	_flushTimer.start();
	long sleepTime = 100; // milliseconds
	while (!_stop)
	{
		try
		{
			sleepTime = 100;
			if (_reconnect)
			{
				close();
				open();
				_reconnect = !_connector.empty() && _pSession.isNull();
				if (_reconnect && sleepTime < 12800)
					sleepTime *= 2;
			}
			if (!_reconnect)
			{
				if (_logQueue.size()) processBatch(_minBatch);
				if (shouldFlush()) processBatch();
				sleepTime = 100;
			}
		}
		catch (const Poco::Exception& ex)
		{
			if (!_stop)
				_logger.error(ex.displayText());
			else
				logLocal(ex.displayText());
		}
		catch (const std::exception& ex)
		{
			if (!_stop)
				_logger.error(ex.what());
			else
				logLocal(ex.what());
		}
		catch (...)
		{
			if (!_stop)
				_logger.error("SQLChannel::run(): unknown exception"s);
			else
				logLocal("SQLChannel::run(): unknown exception"s);
		}
		if (_stop)
		{
			break;
		}
		_event.tryWait(sleepTime);
	}
	while (_logQueue.size() || _source.size())
		processBatch();
}


void SQLChannel::stop()
{
	if (_pLogThread)
	{
		_reconnect = false;
		_stop = true;
		while (_pLogThread->isRunning())
			Thread::sleep(10);
		_pLogThread->join();
		_pLogThread.reset();
		_event.set();
	}
}


void SQLChannel::reconnect()
{
	_reconnect = true;
	if (!_pLogThread)
	{
		_pLogThread = std::make_unique<Thread>();
		_pLogThread->start(*this);
	}
}


size_t SQLChannel::logToFile(bool flush)
{
	if (_source.empty()) return 0u;

	static std::vector<std::string> names;
	if (names.size() != _source.size())
		names.resize(_source.size(), Poco::replace(_name, "'", "''"));

	std::size_t n = 0, batch = 0;

	AutoPtr<FileChannel> pFileChannel;
	std::string file = _file;
	if (!_pFileChannel && !_file.empty())
	{
		if (!Path(File(file).path()).isAbsolute())
			file = _directory + file;

		_pFileChannel = new FileChannel(file);
		pFileChannel = _pFileChannel;
	}
	else
	{
		UUID uuid = UUIDGenerator::defaultGenerator().createRandom();
		std::string filename(_directory);
		filename.append(DateTimeFormatter::format(LocalDateTime(), "%Y%m%d%H%M%S%i.").append(uuid.toString()).append(".log.sql"s));
		pFileChannel = new FileChannel(filename);
	}

	std::stringstream os;
	auto doLog = [&]
	{
		Message msg(_source[0], os.str(), Message::PRIO_FATAL);
		pFileChannel->log(msg);
		n += batch;
		_logged += batch;
		_source.erase(_source.begin(), _source.begin()+batch);
		_pid.erase(_pid.begin(), _pid.begin() + batch);
		_thread.erase(_thread.begin(), _thread.begin() + batch);
		_tid.erase(_tid.begin(), _tid.begin() + batch);
		_priority.erase(_priority.begin(), _priority.begin() + batch);
		_text.erase(_text.begin(), _text.begin() + batch);
		_dateTime.erase(_dateTime.begin(), _dateTime.begin() + batch);
		_flushTimer.restart();
	};

	if (pFileChannel)
	{
		std::string sql;
		Poco::format(sql, SQL_INSERT_STMT, _table, std::string());
		std::stringstream tmp;
		os << sql << '\n';
		auto it = _source.begin();
		auto end = _source.end();
		int idx = 0;
		for (; it != end; ++idx)
		{
			std::string dt = DateTimeFormatter::format(_dateTime[idx], "%Y-%m-%d %H:%M:%S.%i");
			tmp.str("");
			tmp << "('" << *it << "','" <<
				names[idx] << "'," <<
				_pid[idx] << ",'" <<
				_thread[idx] << "'," <<
				_tid[idx] << ',' <<
				_priority[idx] << ",'" <<
				_text[idx] << "','" <<
				dt << "')";

			if (++batch == _maxBatch || (os.str().length() + tmp.str().length()) >= _maxSQL)
			{
				os << ";\n";
				doLog();
				os.str(""); sql.clear();
				Poco::format(sql, SQL_INSERT_STMT, _table, std::string());
				os << sql << '\n' << tmp.str();
				batch = 0;
			}

			os << tmp.str();

			if (++it == end)
			{
				os << ";\n";
				break;
			}
			os << ",\n";
		}

		if ((batch >= _minBatch) || flush) doLog();
	}

	return n;
}


size_t SQLChannel::logToDB(bool flush)
{
	if (_source.empty()) return 0u;

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

	std::size_t n = 0;

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
			// most likely bulk mode not supported, try again
			catch (const Poco::InvalidAccessException&)
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
	catch (const Poco::Exception& ex)
	{
		logLocal(ex.displayText());
		close();
		_reconnect = true;
	}
	catch (const std::exception& ex)
	{
		logLocal(ex.what());
		close();
		_reconnect = true;
	}

	if (n)
	{
		_logged += n;
		_source.erase(_source.begin(), _source.begin() + n);
		_pid.erase(_pid.begin(), _pid.begin() + n);
		_thread.erase(_thread.begin(), _thread.begin() + n);
		_tid.erase(_tid.begin(), _tid.begin() + n);
		_priority.erase(_priority.begin(), _priority.begin() + n);
		_text.erase(_text.begin(), _text.begin() + n);
		_dateTime.erase(_dateTime.begin(), _dateTime.begin() + n);
		_flushTimer.restart();
	}

	return n;
}


size_t SQLChannel::execSQL(bool flush)
{
	if (!_connector.empty() && (!_pSession || !_pSession->isConnected())) open();
	if (_pArchiveStrategy) _pArchiveStrategy->archive();

	size_t n = _pSession ? logToDB(flush) : logToFile(flush);

	return n;
}


std::size_t SQLChannel::wait(int ms)
{
	Stopwatch sw;
	sw.start();
	while (_logQueue.size())
	{
		Thread::sleep(10);
		if (ms && sw.elapsed() * 1000 > ms)
			break;
	}
	return _logQueue.size();
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
			_pArchiveStrategy = nullptr;
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
			_pArchiveStrategy = nullptr;
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
		// no-op, deprecated
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
	else if (name == PROP_MAX_SQL)
	{
		int maxSQL = NumberParser::parse(value);
		_maxSQL = maxSQL;
	}
	else if (name == PROP_BULK)
	{
		_bulk = isTrue(value);
	}
	else if (name == PROP_THROW)
	{
		_throw = isTrue(value);
	}
	else if (name == PROP_DIRECTORY)
	{
		std::string dir = value;
		if (!Path(File(dir).path()).isAbsolute())
		{
			Path d(dir);
			dir = d.makeDirectory().makeAbsolute().toString();
			File f(dir);
			if (!f.exists()) f.createDirectories();
		}
		_directory = dir;
	}
	else if (name == PROP_FILE)
	{
		_file = value;
	}
	else if (name == PROP_FLUSH)
	{
		_flush.store(NumberParser::parse(value));
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
	else if (name == PROP_MAX_SQL)
	{
		return std::to_string(_maxSQL);
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
	else if (name == PROP_DIRECTORY)
	{
		return _directory;
	}
	else if (name == PROP_FILE)
	{
		return _file;
	}
	else if (name == PROP_FLUSH)
	{
		return std::to_string(_flush);
	}
	else
	{
		return Channel::getProperty(name);
	}
}


void SQLChannel::registerChannel()
{
	Poco::LoggingFactory::defaultFactory().registerChannelClass("SQLChannel",
		new Poco::Instantiator<SQLChannel, Poco::Channel>);
}


} } // namespace Poco::Data
