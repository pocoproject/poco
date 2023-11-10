//
// Session.cpp
//
// Library: Data
// Package: DataCore
// Module:  Session
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/Session.h"
#include "Poco/Data/SessionFactory.h"
#include "Poco/String.h"
#include "Poco/URI.h"
#include <algorithm>


namespace Poco {
namespace Data {


Session::Session(Poco::AutoPtr<SessionImpl> pImpl):
	_pImpl(pImpl),
	_statementCreator(pImpl),
	_wasAutoCommit(false)
{
}


Session::Session(const std::string& connector,
	const std::string& connectionString,
	std::size_t timeout)
{
	Session newSession(SessionFactory::instance().create(connector, connectionString, timeout));
	swap(newSession);
}


Session::Session(const std::string& connection, std::size_t timeout)
{
	Session newSession(SessionFactory::instance().create(connection, timeout));
	swap(newSession);
}


Session::Session(const Session& other):
	_pImpl(other._pImpl),
	_statementCreator(other._statementCreator),
	_wasAutoCommit(other._wasAutoCommit)
{
}


Session::Session(Session&& other) noexcept:
	_pImpl(std::move(other._pImpl)),
	_statementCreator(std::move(other._statementCreator)),
	_wasAutoCommit(other._wasAutoCommit)
{
	other._pImpl = nullptr;
	other._statementCreator.reset();
	other._wasAutoCommit = false;
}

Session::~Session()
{
}


Session& Session::operator = (const Session& other)
{
	Session tmp(other);
	swap(tmp);
	return *this;
}


Session& Session::operator = (Session&& other) noexcept
{
	_pImpl = std::move(other._pImpl);
	_statementCreator = std::move(other._statementCreator);
	_wasAutoCommit = other._wasAutoCommit;
	return *this;
}


void Session::swap(Session& other)
{
	using std::swap;
	swap(_statementCreator, other._statementCreator);
	swap(_pImpl, other._pImpl);
	swap(_wasAutoCommit, other._wasAutoCommit);
}


void Session::begin()
{
	if (isAutocommit())
	{
		setFeature("autoCommit", false);
		_wasAutoCommit = true;
	}
	return _pImpl->begin();
}


void Session::commit()
{
	_pImpl->commit();
	if (_wasAutoCommit)
	{
		setFeature("autoCommit", true);
		_wasAutoCommit = false;
	}
}


void Session::rollback()
{
	_pImpl->rollback();
	if (_wasAutoCommit)
	{
		setFeature("autoCommit", true);
		_wasAutoCommit = false;
	}
}


} } // namespace Poco::Data
