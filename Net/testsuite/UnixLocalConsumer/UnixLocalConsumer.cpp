//
// UnixLocalConsumer.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SocketAddress.h"


// UNIX_LOCAL exists only with UNIX sockets enabled and afunix.h in the SDK.
// The condition must not depend on POCO_HAVE_CPP17_COMPILER, which the
// assertion checks.
#if !defined(POCO_NET_NO_UNIX_SOCKET) && __has_include(<afunix.h>)
// Compiles only if this file evaluates the feature macros of Poco/Config.h
// as the Net library does, which is built with UNIX_LOCAL on Windows.
static_assert(Poco::Net::SocketAddress::UNIX_LOCAL != Poco::Net::SocketAddress::IPv4);
#endif
