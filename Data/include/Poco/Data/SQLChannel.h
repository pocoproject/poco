//
// SQLChannel.h
//
// Library: Data
// Package: Logging
// Module:  SQLChannel
//
// Definition of the SQLChannel class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_SQLChannel_INCLUDED
#define Data_SQLChannel_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/Statement.h"
#include "Poco/Logger.h"
#include "Poco/Data/ArchiveStrategy.h"
#include "Poco/Channel.h"
#include "Poco/FileChannel.h"
#include "Poco/Message.h"
#include "Poco/AutoPtr.h"
#include "Poco/String.h"
#include "Poco/NotificationQueue.h"
#include "Poco/Thread.h"
#include "Poco/Stopwatch.h"
#include "Poco/Event.h"
#include <atomic>
#include <atomic>


namespace Poco {
namespace Data {


class Data_API SQLChannel: public Poco::Channel, Poco::Runnable
	/// This Channel implements logging to a SQL database.
	/// The channel is dependent on the schema. The DDL for
	/// table creation (subject to target DDL dialect dependent
	/// modifications) is:
	///
	/// "CREATE TABLE T_POCO_LOG (Source VARCHAR,
	///		Name VARCHAR,
	///		ProcessId INTEGER,
	///		Thread VARCHAR,
	///		ThreadId INTEGER,
	///		Priority INTEGER,
	///		Text VARCHAR,
	///		DateTime DATE)"
	///
	/// The table name is configurable through "table" property.
	/// Other than DateTime filed name used for optional time-based archiving purposes, currently the
	/// field names are not mandated. However, it is recommended to use names as specified above.
	///
	/// To provide as non-intrusive operation as possible, the log entries are cached and
	/// inserted into the target database asynchronously by default. The blocking, however, will occur
	/// before the next entry insertion with default timeout of 1 second. The default settings can be
	/// overridden (see async, timeout and throw properties for details).
	/// If throw property is false, insertion timeouts are ignored, otherwise a TimeoutException is thrown.
	/// To force insertion of every entry, set timeout to 0. This setting, however, introduces
	/// a risk of long blocking periods in case of remote server communication delays.
	///
	/// A default-constructed SQLChannel operates without an active DB connection, in a store-and-forward
	/// mode. For this mode of operation, a separate service is required to consume and execute the SQL
	/// statements from the stored files and insert the log entries into the database. Since this setup
	/// stores SQL inserts in the OS file system, it is strongly recommended to take the necessary
	/// precautions to limit and secure the access to those files.
{
public:
	class LogNotification : public Poco::Notification
	{
	public:
		using Ptr = Poco::AutoPtr<LogNotification>;

		LogNotification(const Poco::Message& message) :
			_message(message)
		{
		}

		const Poco::Message& message() const
		{
			return _message;
		}

	private:
		Poco::Message _message;
	};

	static const int DEFAULT_MIN_BATCH_SIZE = 1;
	static const int DEFAULT_MAX_BATCH_SIZE = 1000;
	static const int DEFAULT_MAX_SQL_SIZE = 65536;
	static const int DEFAULT_FLUSH_SECONDS = 10;

	SQLChannel();
		/// Creates SQLChannel.
		/// An SQLChannel without DB connector and local file name
		/// only logs SQL inserts into local files. A separate file
		/// is created for each insert batch; files are named
		/// <YYYYMMDDHH24MISSmmm>.<UUID>.log.sql.

	SQLChannel(const std::string& connector,
		const std::string& connect,
		const std::string& name = "-",
		const std::string& table = "T_POCO_LOG",
		int timeout = 1000,
		int minBatch = DEFAULT_MIN_BATCH_SIZE,
		int maxBatch = DEFAULT_MAX_BATCH_SIZE,
		int maxSQL = DEFAULT_MAX_SQL_SIZE);
		/// Creates an SQLChannel with the given connector, connect string, timeout, table and name.
		/// The connector must be already registered.

	void open() override;
		/// Opens the SQLChannel.
		/// Returns true if succesful.

	void close() override;
		/// Closes the SQLChannel.

	void run() override;
		/// Dequeues and sends the logs to the DB.

	bool isRunning() const;
		/// Returns true if the logging thread is running.

	void log(const Message& msg) override;
		/// Writes the log message to the database.

	void setProperty(const std::string& name, const std::string& value) override;
		/// Sets the property with the given value.
		///
		/// The following properties are supported:
		///     * name:      The name used to identify the source of log messages.
		///                  Defaults to "-".
		///
		///     * connector: The target data storage connector name.
		///
		///     * connect:   The target data storage connection string.
		///
		///     * table:     Destination log table name. Defaults to "T_POCO_LOG".
		///                  Table must exist in the target database.
		///
		///     * keep:      Max row age for the log table. To disable archiving,
		///                  set this property to empty string or "forever".
		///
		///     * archive:   Archive table name. Defaults to "T_POCO_LOG_ARCHIVE".
		///                  Table must exist in the target database. To disable archiving,
		///                  set this property to empty string.
		///
		///     * async:     Indicates asynchronous execution. When excuting asynchronously,
		///                  messages are sent to the target using asyncronous execution.
		///                  However, prior to the next message being processed and sent to
		///                  the target, the previous operation must have been either completed
		///                  or timed out (see timeout and throw properties for details on
		///                  how abnormal conditos are handled).
		///                  This property is deprecated and has no effect - all logging
		///                  is asynchronous since the 1.13.0. release.
		///
		///     * timeout:   Timeout (ms) to wait for previous log operation completion.
		///                  Values "0" and "" mean no timeout.
		///
		///     * throw:     Boolean value indicating whether to throw in case of timeout.
		///                  Setting this property to false may result in log entries being lost.
		///                  True values are (case insensitive) "true", "t", "yes", "y".
		///                  Anything else yields false.
		///
		///     * minBatch:  Minimal number of log entries to accumulate before actually sending
		///                  logs to the destination.
		///                  Defaults to 1 entry (for compatibility with older versions);
		///                  can't be zero or larger than `maxBatch`.
		///
		///     * maxBatch:  Maximum number of log entries to accumulate. When the log queue
		///                  reaches this size, log entries are silently discarded.
		///                  Defaults to 100, can't be zero or larger than 1000.
		///
		///     * maxSQL:    Maximum total length of the SQL statement. Defaults to 65536.
		///
		///     * bulk:      Do bulk execute (on most DBMS systems, this can speed up things
		///                  drastically).
		///
		///     * file       Destination file name for the backup FileChannel, used when DB
		///                  connection is not present to log not executed SQL statements.
		///
		///     * flush      Time in seconds to flush outstanding log entries; since logging
		///                  is performed in batches of entries, the entries that do not make
		///                  it into the last logged batch may remain unlogged for a long time
		///                  during an extened period of inactivity. This setting ensures that
		///                  unlogged entries are flushed in such circumstances, even when the
		///                  minimum batch size was not reached.
		///                  Zero value means no flushing.

	std::string getProperty(const std::string& name) const override;
		/// Returns the value of the property with the given name.

	void stop();
		/// Stops and joins the logging thread..

	std::size_t wait(int ms = 1000);
		/// Waits for the completion of the previous operation and returns
		/// the result. If chanel is in synchronous mode, returns 0 immediately.

	size_t logged() const;
		/// Returns the number of logged entries.

	static void registerChannel();
		/// Registers the channel with the global LoggingFactory.

	static const std::string PROP_CONNECT;
	static const std::string PROP_CONNECTOR;
	static const std::string PROP_NAME;
	static const std::string PROP_TABLE;
	static const std::string PROP_ARCHIVE_TABLE;
	static const std::string PROP_MAX_AGE;
	POCO_DEPRECATED("") static const std::string PROP_ASYNC;
	static const std::string PROP_TIMEOUT;
	static const std::string PROP_MIN_BATCH;
	static const std::string PROP_MAX_BATCH;
	static const std::string PROP_MAX_SQL;
	static const std::string PROP_BULK;
	static const std::string PROP_THROW;
	static const std::string PROP_DIRECTORY;
	static const std::string PROP_FILE;
	static const std::string PROP_FLUSH;

protected:
	~SQLChannel() override;

private:
	static const std::string SQL_INSERT_STMT;

	using SessionPtr = Poco::SharedPtr<Session>;
	using StatementPtr = Poco::SharedPtr<Statement>;
	using Priority = Poco::Message::Priority;
	using StrategyPtr = Poco::SharedPtr<ArchiveStrategy>;

	void reconnect();
		/// Closes and opens the DB connection.

	bool processBatch(int minBatch = 0);
		/// Processes a batch of messages.
		/// If the number of acummulated messages is greater
		/// than minBatch, sends logs to the destination.
		/// Returns true if at least one log entry was sent
		/// to the destination.

	size_t execSQL(bool flush = false);
		/// Executes the log statement.

	size_t logSync(bool flush = false);
		/// Inserts entries into the target database.

	bool isTrue(const std::string& value) const;
		/// Returns true is value is "true", "t", "yes" or "y".
		/// Case insensitive.

	size_t logToDB(bool flush = false);
		/// Logs cached entries to the DB.

	size_t logToFile(bool flush = false);
		/// Logs cached entries to a file.
		/// Called in case DB insertions fail or
		/// in the store-and-forward mode of operation.

	void logLocal(const std::string&, Message::Priority prio = Message::PRIO_ERROR);
		/// Adds the message to the local SQLChannel log queue, and logs it to the file.
		/// Typically used to log DB connection/execution erors.

	std::string maskPwd();
		/// Masks the password in the connection
		/// string, if detected. This is not a
		/// bullet-proof method; if not succesful,
		/// empty string is returned.

	bool shouldFlush() const;
		/// Returns true if there are unflushed log entries
		/// and the flush timer has expired.

	mutable Poco::FastMutex _mutex;

	std::string       _connector;
	std::string       _connect;
	SessionPtr        _pSession;
	std::string       _sql;
	std::string       _name;
	std::string       _table;
	std::atomic<bool> _tableChanged;
	std::atomic<int>  _timeout;
	std::atomic<int>  _minBatch;
	std::atomic<int>  _maxBatch;
	std::atomic<int>  _maxSQL;
	std::atomic<bool> _bulk;
	std::atomic<bool> _throw;
	std::atomic<int>  _flush;

	// members for log entry cache
	std::deque<std::string>      _source;
	std::deque<long>             _pid;
	std::deque<std::string>      _thread;
	std::deque<long>             _tid;
	std::deque<int>              _priority;
	std::deque<std::string>      _text;
	std::deque<DateTime>         _dateTime;
	Poco::NotificationQueue       _logQueue;
	std::unique_ptr<Poco::Thread> _pLogThread;
	std::atomic<bool>             _reconnect;
	std::atomic<bool>             _stop;
	std::atomic<size_t>           _logged;
	StrategyPtr                   _pArchiveStrategy;
	std::string                   _file;
	std::string                   _directory;
	AutoPtr<FileChannel>          _pFileChannel;
	Poco::Stopwatch               _flushTimer;
	Poco::Event                   _event;
	Poco::Logger& _logger = Poco::Logger::get("SQLChannel");
};


//
// inlines
//


inline bool SQLChannel::isTrue(const std::string& value) const
{
	return ((0 == icompare(value, "true")) ||
			(0 == icompare(value, "t")) ||
			(0 == icompare(value, "yes")) ||
			(0 == icompare(value, "y")));
}


inline bool SQLChannel::isRunning() const
{
	return _pLogThread && _pLogThread->isRunning();
}


inline size_t SQLChannel::logged() const
{
	return _logged;
}


inline bool SQLChannel::shouldFlush() const
{
	return (_flush > 0 && _source.size() &&
		(_flushTimer.elapsedSeconds() >= _flush));
}


} } // namespace Poco::Data


#endif // Data_SQLChannel_INCLUDED
