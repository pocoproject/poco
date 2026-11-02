//
// Notifier.h
//
// Library: Data/SQLite
// Package: SQLite
// Module:  Notifier
//
// Definition of Notifier.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQLite_Notifier_INCLUDED
#define SQLite_Notifier_INCLUDED


#include "Poco/Data/SQLite/SQLite.h"
#include "Poco/Data/SQLite/Utility.h"
#include "Poco/Data/Session.h"
#include "Poco/Mutex.h"
#include "Poco/Types.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/BasicEvent.h"
#include <map>


namespace Poco::Data::SQLite {


class SQLite_API Notifier
	/// Notifier is a wrapper for SQLite callback calls. It supports event callbacks
	/// for insert, update, delete, commit and rollback events. While (un)registering
	/// callbacks is thread-safe, execution of the callbacks themselves are not;
	/// it is the user's responsibility to ensure the thread-safey of the functions
	/// they provide as callback target. Additionally, commit callbacks may prevent
	/// database transactions from succeeding (see sqliteCommitCallbackFn documentation
	/// for details).
	///
	/// There can be only one set of callbacks per session (i.e. registering a new
	/// callback automatically unregisters the previous one). All callbacks are
	/// registered and enabled at Notifier contruction time and can be disabled
	/// at a later point time.
	///
	/// The callbacks are registered with the connection the session has when they
	/// are enabled. Closing a session removes them from its connection before SQLite
	/// rolls back a transaction still in progress, so that rollback is not notified;
	/// once the session is open again, enable them again. Closing a pooled session
	/// first rolls back a transaction still in progress with an explicit ROLLBACK,
	/// which is notified, and then returns the connection to its pool without the
	/// callbacks; a closed pooled session cannot be opened again, so enable them with
	/// a Notifier on a session taken from the pool. Callbacks removed so are no longer
	/// enabled: updateEnabled(), commitEnabled() and rollbackEnabled() return false.
	/// Enabling them uses the session, which must still exist then; disabling them,
	/// also in the destructor, uses the connection they were registered with, never
	/// the session. The session must not be closed while another thread enables or
	/// disables them.
{
public:
	typedef unsigned char EnabledEventType;
		/// A type definition for events-enabled bitmap.

	typedef Poco::BasicEvent<void> Event;

	//
	// Events
	//
	Event update;
	Event insert;
	Event erase;
	Event commit;
	Event rollback;

	// Event types.
	static constexpr EnabledEventType SQLITE_NOTIFY_UPDATE   = 1;
	static constexpr EnabledEventType SQLITE_NOTIFY_COMMIT   = 2;
	static constexpr EnabledEventType SQLITE_NOTIFY_ROLLBACK = 4;

	Notifier(const Session& session,
		EnabledEventType enabled = SQLITE_NOTIFY_UPDATE | SQLITE_NOTIFY_COMMIT | SQLITE_NOTIFY_ROLLBACK);
			/// Creates a Notifier and enables all callbacks.
			/// Throws NullPointerException if a callback is to be enabled and the session is not connected.

	Notifier(const Session& session,
		const Any& value,
		EnabledEventType enabled = SQLITE_NOTIFY_UPDATE | SQLITE_NOTIFY_COMMIT | SQLITE_NOTIFY_ROLLBACK);
			/// Creates a Notifier, assigns the value to the internal storage and and enables all callbacks.
			/// Throws NullPointerException if a callback is to be enabled and the session is not connected.

	~Notifier();
		/// Disables all callbacks and destroys the Notifier.

	bool enableUpdate();
		/// Enables update callbacks.
		/// Throws NullPointerException if the session is not connected.

	bool disableUpdate();
		/// Disables update callbacks.

	[[nodiscard]] bool updateEnabled() const;
		/// Returns true if update callbacks are enabled, false otherwise.

	bool enableCommit();
		/// Enables commit callbacks.
		/// Throws NullPointerException if the session is not connected.

	bool disableCommit();
		/// Disables commit callbacks.

	[[nodiscard]] bool commitEnabled() const;
		/// Returns true if update callbacks are enabled, false otherwise.

	bool enableRollback();
		/// Enables rollback callbacks.
		/// Throws NullPointerException if the session is not connected.

	bool disableRollback();
		/// Disables rollback callbacks.

	[[nodiscard]] bool rollbackEnabled() const;
		/// Returns true if rollback callbacks are enabled, false otherwise.

	bool enableAll();
		/// Enables all callbacks.
		/// Throws NullPointerException if the session is not connected.

	bool disableAll();
		/// Disables all callbacks.

	static void sqliteUpdateCallbackFn(void* pVal, int opCode, const char* pDB, const char* pTable, long long row);
		/// Update callback event dispatcher. Determines the type of the event, updates the row number
		/// and triggers the event.
		/// An exception thrown by an event handler is passed to Poco::ErrorHandler; it does not reach SQLite.

	static int sqliteCommitCallbackFn(void* pVal);
		/// Commit callback event dispatcher. If an exception occurs, it is catched inside this function,
		/// non-zero value is returned, which causes SQLite engine to turn commit into a rollback.
		/// Therefore, callers should check for return value - if it is zero, callback completed succesfuly
		/// and transaction was committed.

	static void sqliteRollbackCallbackFn(void* pVal);
		/// Rollback callback event dispatcher.
		/// An exception thrown by an event handler is passed to Poco::ErrorHandler; it does not reach SQLite.

	[[nodiscard]] bool operator == (const Notifier& other) const;
		/// Equality operator. Compares value, row and the database handles the
		/// callbacks are registered with, and returns true iff all are equal.

	[[nodiscard]] const std::string& getTable() const;
		/// Returns the table name.

	void setTable(const std::string& table);
		/// Sets the row number.

	[[nodiscard]] Poco::Int64 getRow() const;
		/// Returns the row number.

	void setRow(Poco::Int64 row);
		/// Sets the row number.

	[[nodiscard]] const Poco::Dynamic::Var& getValue() const;
		/// Returns the value.

	template <typename T>
	inline void setValue(const T& val)
		/// Sets the value.
	{
		_value = val;
	}

private:
	Notifier() = delete;
	Notifier(const Notifier&) = delete;
	Notifier& operator=(const Notifier&) = delete;

	void init(EnabledEventType enabled);
		/// Enables the given callbacks. If one of them cannot be enabled,
		/// disables those already enabled and rethrows.

	void bindConnection();
		/// Makes the session's current connection the one the callbacks are
		/// registered with, disabling them on the previous one, and forgets the
		/// callbacks that closing or resetting the session has removed.
		/// Throws NullPointerException if the session is not connected.

	const Session&     _session;
	sqlite3*           _pDB = nullptr;
	std::string        _table;
	Poco::Int64        _row = 0;
	Poco::Dynamic::Var _value;
	EnabledEventType   _enabledEvents = 0;
	Poco::Mutex        _mutex;
};


//
// inlines
//

inline bool Notifier::operator == (const Notifier& other) const
{
	return _value == other._value &&
		_row == other._row &&
		_pDB == other._pDB;
}


inline const std::string& Notifier::getTable() const
{
	return _table;
}


inline void Notifier::setTable(const std::string& table)
/// Sets the row number.
{
	_table = table;
}


inline Poco::Int64 Notifier::getRow() const
{
	return _row;
}


inline void Notifier::setRow(Poco::Int64 row)
{
	_row = row;
}


inline const Poco::Dynamic::Var& Notifier::getValue() const
{
	return _value;
}


} // namespace Poco::Data::SQLite


#endif // SQLite_Notifier_INCLUDED
