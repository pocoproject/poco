//
// PooledSessionHolder.cpp
//
// $Id: //poco/Main/Data/src/PooledSessionHolder.cpp#1 $
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


#include "Poco/Data/PooledSessionHolder.h"


namespace Poco {
namespace Data {


PooledSessionHolder::PooledSessionHolder(SessionPool& rOwner, SessionImpl* pSessionImpl):
	_owner(rOwner),
	_pImpl(pSessionImpl, true)
{
}


PooledSessionHolder::~PooledSessionHolder()
{
}


} } // namespace Poco::Data
