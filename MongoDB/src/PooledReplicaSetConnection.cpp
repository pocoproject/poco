//
// PooledReplicaSetConnection.cpp
//
// Library: MongoDB
// Package: MongoDB
// Module:  PooledReplicaSetConnection
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/MongoDB/ReplicaSetPoolableConnectionFactory.h"


namespace Poco::MongoDB {


PooledReplicaSetConnection::PooledReplicaSetConnection(PooledReplicaSetConnection&& other) noexcept = default;


PooledReplicaSetConnection& PooledReplicaSetConnection::operator=(PooledReplicaSetConnection&& other) noexcept = default;


} // namespace Poco::MongoDB
