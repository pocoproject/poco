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


#include "Poco/Data/StatementCreator.h"
#include <algorithm>
#include <utility>


namespace Poco {
namespace Data {


StatementCreator::StatementCreator()
{
}


StatementCreator::StatementCreator(Poco::AutoPtr<SessionImpl> ptrImpl):
	_ptrImpl(std::move(ptrImpl))
{
}


StatementCreator::StatementCreator(const StatementCreator& other):
	_ptrImpl(other._ptrImpl)
{
}


StatementCreator::StatementCreator(StatementCreator&& other) noexcept:
	_ptrImpl(std::move(other._ptrImpl))
{
	other._ptrImpl = nullptr;
}


StatementCreator& StatementCreator::operator = (const StatementCreator& other)
{
	StatementCreator tmp(other);
	swap(tmp);
	return *this;
}


StatementCreator& StatementCreator::operator = (StatementCreator&& other) noexcept
{
	_ptrImpl = std::move(other._ptrImpl);
	other._ptrImpl = nullptr;
	return *this;
}


StatementCreator::~StatementCreator()
{
}


} } // namespace Poco::Data
