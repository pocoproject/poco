//
// StatementCreator.cpp
//
// Library: Data
// Package: DataCore
// Module:  StatementCreator
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/StatementCreator.h"
#include <algorithm>


namespace Poco {
namespace SQL {


StatementCreator::StatementCreator()
{
}


StatementCreator::StatementCreator(SessionImpl::Ptr ptrImpl):
	_ptrImpl(ptrImpl)
{
}


StatementCreator::StatementCreator(const StatementCreator& other):
	_ptrImpl(other._ptrImpl)
{
}


StatementCreator& StatementCreator::operator = (const StatementCreator& other)
{
	if (this != &other)
	{
		StatementCreator tmp(other);
		swap(tmp);
	}
	return *this;
}


StatementCreator& StatementCreator::operator = (Poco::AutoPtr<SessionImpl> ptrImpl)
{
	_ptrImpl = ptrImpl;
	return *this;
}


void StatementCreator::swap(StatementCreator& other)
{
	using std::swap;
	swap(_ptrImpl, other._ptrImpl);
}


StatementCreator::~StatementCreator()
{
}


} } // namespace Poco::SQL
