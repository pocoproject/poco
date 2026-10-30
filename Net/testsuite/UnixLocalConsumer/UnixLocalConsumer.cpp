//
// UnixLocalConsumer.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SocketAddress.h"


// Compiles only if this file evaluates the feature macros of Poco/Config.h
// as the Net library does, which is built with UNIX_LOCAL on Windows.
static_assert(Poco::Net::SocketAddress::UNIX_LOCAL != Poco::Net::SocketAddress::IPv4);
