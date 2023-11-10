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
#include "Poco/Data/Connector.h"
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
#include "Poco/Mutex.h"
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
	/// field names are not mandated. However, it is recomended to use names as specified above.
	///
	/// To provide as non-intrusive operation as possbile, the log entries are cached and
	/// inserted into the target database asynchronously by default. The blocking, however, will occur
	/// before the next entry insertion with default timeout of 1 second. The default settings can be
	/// overridden (see async, timeout and throw properties for details).
	/// If throw property is false, insertion timeouts are ignored, otherwise a TimeoutException is thrown.
	/// To force insertion of every entry, set timeout to 0. This setting, however, introduces
	/// a risk of long blocking periods in case of remote server communication delays.
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

	SQLChannel();
		/// Creates SQLChannel.

	SQLChannel(const std::string& connector,
		const std::string& connect,
		const std::string& name = "-",
		const std::string& table = "T_POCO_LOG",
		int timeout = 1000,
		int minBatch = DEFAULT_MIN_BATCH_SIZE,
		int maxBatch = DEFAULT_MAX_BATCH_SIZE);
		/// Creates an SQLChannel with the given connector, connect string, timeout, table and name.
		/// The connector must be already registered.

	void open();
		/// Opens the SQLChannel.
		/// Returns true if succesful.

	void close();
		/// Closes the SQLChannel.

	void run();
		/// Dequeues and sends the logs to the DB.

	bool isRunning() const;
		/// Returns true if the logging thread is running.

	void log(const Message& msg);
		/// Writes the log message to the database.

	void setProperty(const std::string& name, const std::string& value);
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
		///                  Values "0" and "" mean no timeout. Only valid when logging
		///                  is asynchronous, otherwise ignored.
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
		///     * bulk:      Do bulk execute (on most DBMS systems, this can speed up things
		///                  drastically).
		///
		///     * file       Destination file name for the backup FileChannel, used when DB
		///                  connection is not present to log not executed SQL statements.

	std::string getProperty(const std::string& name) const;
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
	static const std::string PROP_ASYNC;
	static const std::string PROP_TIMEOUT;
	static const std::string PROP_MIN_BATCH;
	static const std::string PROP_MAX_BATCH;
	static const std::string PROP_BULK;
	static const std::string PROP_THROW;
	static const std::string PROP_FILE;

protected:
	~SQLChannel();

private:
	static const std::string SQL_INSERT_STMT;

	typedef Poco::SharedPtr<Session>         SessionPtr;
	typedef Poco::SharedPtr<Statement>       StatementPtr;
	typedef Poco::Message::Priority          Priority;
	typedef Poco::SharedPtr<ArchiveStrategy> StrategyPtr;

	void reconnect();
		/// Closes and opens the DB connection.

	bool processOne(int minBatch = 0);
		/// Processes one message.
		/// If the number of acummulated messages is greater
		/// than minBatch, sends logs to the destination.
		/// Returns true if log entry was processed.

	size_t execSQL();
		/// Executes the log statement.

	size_t logSync();
		/// Inserts entries into the target database.

	bool isTrue(const std::string& value) const;
		/// Returns true is value is "true", "t", "yes" or "y".
		/// Case insensitive.

	size_t logTofile(AutoPtr<FileChannel>& pFileChannel, const std::string& fileName, bool clear = false);
		/// Logs cached entries to a file. Called in case DB insertions fail.

	std::string maskPwd();
		/// Masks the password in the connection
		/// string, if detected. This is not a
		/// bullet-proof method; if not succesful,
		/// empty string is returned.

	mutable Poco::FastMutex _mutex;

	std::string      _connector;
	std::string      _connect;
	SessionPtr       _pSession;
	std::string      _sql;
	std::string      _name;
	std::string      _table;
	bool             _tableChanged;
	int              _timeout;
	std::atomic<int> _minBatch;
	int              _maxBatch;
	bool             _bulk;
	std::atomic<bool> _throw;

	// members for log entry cache
	std::vector<std::string>      _source;
	std::vector<long>             _pid;
	std::vector<std::string>      _thread;
	std::vector<long>             _tid;
	std::vector<int>              _priority;
	std::vector<std::string>      _text;
	std::vector<DateTime>         _dateTime;
	Poco::NotificationQueue       _logQueue;
	std::unique_ptr<Poco::Thread> _pDBThread;
	std::atomic<bool>             _reconnect;
	std::atomic<bool>             _running;
	std::atomic<bool>             _stop;
	std::atomic<size_t>           _logged;
	StrategyPtr                   _pArchiveStrategy;
	std::string                   _file;
	AutoPtr<FileChannel>          _pFileChannel;
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
	return _running;
}


inline size_t SQLChannel::logged() const
{
	return _logged;
}


} } // namespace Poco::Data


#endif // Data_SQLChannel_INCLUDED
