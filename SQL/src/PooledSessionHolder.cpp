//
// PooledSessionHolder.cpp
//
// Library: Data
// Package: SessionPooling
// Module:  PooledSessionHolder
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SQL/PooledSessionHolder.h"


namespace Poco {
namespace SQL {


PooledSessionHolder::PooledSessionHolder(SessionPool& rOwner, SessionImpl::Ptr pSessionImpl):
	_owner(rOwner),
	_pImpl(pSessionImpl)
{
}


PooledSessionHolder::~PooledSessionHolder()
{
}


} } // namespace Poco::SQL
