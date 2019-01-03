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


#include "Poco/SQL/Session.h"
#include "Poco/SQL/SessionFactory.h"
#include "Poco/String.h"
#include "Poco/URI.h"
#include <algorithm>


namespace Poco {
namespace SQL {


Session::Session(Poco::AutoPtr<SessionImpl> pImpl):
	_pImpl(pImpl),
	_statementCreator(pImpl)
{
	poco_check_ptr (pImpl.get());
}


Session::Session(const std::string& rConnector,
	const std::string& connectionString,
	std::size_t timeout)
{
	operator =(SessionFactory::instance().create(rConnector, connectionString, timeout));
}


Session::Session(const std::string& connection,
	std::size_t timeout)
{
	operator =(SessionFactory::instance().create(connection, timeout));
}


Session::Session(const Session& other): _pImpl(other._pImpl),
	_statementCreator(other._pImpl)
{
}


Session::Session(Session&& other): _pImpl(other._pImpl),
	_statementCreator(other._pImpl)
{
	other._pImpl = nullptr;
}


Session::~Session()
{
	if (_pImpl) _pImpl->putBack();
}


Session& Session::operator = (const Session& other)
{
	if (this != &other)
	{
		Session tmp(other);
		swap(tmp);
	}
	return *this;
}


Session& Session::operator = (Session&& other)
{
	if (this != &other)
	{
		_pImpl = other._pImpl;
		_statementCreator = _pImpl;
		other._pImpl = nullptr;
	}
	return *this;
}


void Session::swap(Session& other)
{
	using std::swap;
	swap(_statementCreator, other._statementCreator);
	swap(_pImpl, other._pImpl);
}


} } // namespace Poco::SQL
