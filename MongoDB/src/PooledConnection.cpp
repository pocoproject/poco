//
// PooledConnection.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  PooledConnection
//
// Copyright (c) 2012-2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/PoolableConnectionFactory.h"


namespace Poco::MongoDB {


PooledConnection::PooledConnection(PooledConnection&& other) = default;


PooledConnection& PooledConnection::operator=(PooledConnection&&) = default;


} // namespace Poco::MongoDB
