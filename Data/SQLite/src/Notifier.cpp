//
// Notifier.cpp
//
// Library: Data/SQLite
// Package: SQLite
// Module:  Notifier
//
// Implementation of Notifier
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/SQLite/Notifier.h"
#include "Poco/ErrorHandler.h"


namespace Poco::Data::SQLite {


Notifier::Notifier(const Session& session, EnabledEventType enabled):
	_session(session)
{
	init(enabled);
}


Notifier::Notifier(const Session& session, const Any& value, EnabledEventType enabled):
	_session(session),
	_value(value)
{
	init(enabled);
}


Notifier::~Notifier()
{
	try
	{
		disableAll();
	}
	catch (...)
	{
		poco_unexpected();
	}
}


void Notifier::init(EnabledEventType enabled)
{
	try
	{
		if (enabled & SQLITE_NOTIFY_UPDATE)   enableUpdate();
		if (enabled & SQLITE_NOTIFY_COMMIT)   enableCommit();
		if (enabled & SQLITE_NOTIFY_ROLLBACK) enableRollback();
	}
	catch (...)
	{
		// no destructor runs for a Notifier whose constructor throws
		disableAll();
		throw;
	}
}


void Notifier::bindConnection()
{
	sqlite3* pDB = Utility::dbHandle(_session);
	poco_check_ptr (pDB);
	if (pDB != _pDB)
	{
		disableAll();
		_pDB = pDB;
	}
	// close() and reset() remove the registrations without telling the Notifier,
	// also when the new connection has the address of the old one
	_enabledEvents &= static_cast<EnabledEventType>(Utility::registeredHooks(_pDB, this));
}


bool Notifier::enableUpdate()
{
	Poco::Mutex::ScopedLock l(_mutex);

	bindConnection();
	if (Utility::registerUpdateHandler(_pDB, &sqliteUpdateCallbackFn, this))
		_enabledEvents |= SQLITE_NOTIFY_UPDATE;

	return updateEnabled();
}


bool Notifier::disableUpdate()
{
	Poco::Mutex::ScopedLock l(_mutex);

	// false if not registered (closed, or replaced by another registration): disabled either way
	Utility::registerUpdateHandler(_pDB, static_cast<Utility::UpdateCallbackType>(nullptr), this);
	_enabledEvents &= static_cast<EnabledEventType>(~SQLITE_NOTIFY_UPDATE);

	return !updateEnabled();
}


bool Notifier::updateEnabled() const
{
	return 0 != (_enabledEvents & Utility::registeredHooks(_pDB, this) & SQLITE_NOTIFY_UPDATE);
}


bool Notifier::enableCommit()
{
	Poco::Mutex::ScopedLock l(_mutex);

	bindConnection();
	if (Utility::registerUpdateHandler(_pDB, &sqliteCommitCallbackFn, this))
		_enabledEvents |= SQLITE_NOTIFY_COMMIT;

	return commitEnabled();
}


bool Notifier::disableCommit()
{
	Poco::Mutex::ScopedLock l(_mutex);

	Utility::registerUpdateHandler(_pDB, static_cast<Utility::CommitCallbackType>(nullptr), this);
	_enabledEvents &= static_cast<EnabledEventType>(~SQLITE_NOTIFY_COMMIT);

	return !commitEnabled();
}


bool Notifier::commitEnabled() const
{
	return 0 != (_enabledEvents & Utility::registeredHooks(_pDB, this) & SQLITE_NOTIFY_COMMIT);
}


bool Notifier::enableRollback()
{
	Poco::Mutex::ScopedLock l(_mutex);

	bindConnection();
	if (Utility::registerUpdateHandler(_pDB, &sqliteRollbackCallbackFn, this))
		_enabledEvents |= SQLITE_NOTIFY_ROLLBACK;

	return rollbackEnabled();
}


bool Notifier::disableRollback()
{
	Poco::Mutex::ScopedLock l(_mutex);

	Utility::registerUpdateHandler(_pDB, static_cast<Utility::RollbackCallbackType>(nullptr), this);
	_enabledEvents &= static_cast<EnabledEventType>(~SQLITE_NOTIFY_ROLLBACK);

	return !rollbackEnabled();
}


bool Notifier::rollbackEnabled() const
{
	return 0 != (_enabledEvents & Utility::registeredHooks(_pDB, this) & SQLITE_NOTIFY_ROLLBACK);
}


bool Notifier::enableAll()
{
	return enableUpdate() && enableCommit() && enableRollback();
}


bool Notifier::disableAll()
{
	bool disabled = disableUpdate();
	disabled = disableCommit() && disabled;
	disabled = disableRollback() && disabled;
	return disabled;
}


void Notifier::sqliteUpdateCallbackFn(void* pVal, int opCode, const char* pDB, const char* pTable, long long row)
{
	// an exception must not unwind through SQLite
	try
	{
		poco_check_ptr (pVal);
		auto* pV = static_cast<Notifier*>(pVal);

		if (opCode == Utility::OPERATION_INSERT)
		{
			pV->_table = pTable;
			pV->_row = row;
			pV->insert.notify(pV);
		}
		else if (opCode == Utility::OPERATION_UPDATE)
		{
			pV->_table = pTable;
			pV->_row = row;
			pV->update.notify(pV);
		}
		else if (opCode == Utility::OPERATION_DELETE)
		{
			pV->_table = pTable;
			pV->_row = row;
			pV->erase.notify(pV);
		}
	}
	catch (Poco::Exception& exc)
	{
		ErrorHandler::handle(exc);
	}
	catch (std::exception& exc)
	{
		ErrorHandler::handle(exc);
	}
	catch (...)
	{
		ErrorHandler::handle();
	}
}


int Notifier::sqliteCommitCallbackFn(void* pVal)
{
	Notifier* pV = reinterpret_cast<Notifier*>(pVal);

	try
	{
		pV->commit.notify(pV);
	}
	catch (...)
	{
		return -1;
	}

	return 0;
}


void Notifier::sqliteRollbackCallbackFn(void* pVal)
{
	Notifier* pV = reinterpret_cast<Notifier*>(pVal);

	// an exception must not unwind through SQLite
	try
	{
		pV->rollback.notify(pV);
	}
	catch (Poco::Exception& exc)
	{
		ErrorHandler::handle(exc);
	}
	catch (std::exception& exc)
	{
		ErrorHandler::handle(exc);
	}
	catch (...)
	{
		ErrorHandler::handle();
	}
}


} // namespace Poco::Data::SQLite
