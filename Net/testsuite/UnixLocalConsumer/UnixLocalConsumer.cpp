//
// UnixLocalConsumer.cpp
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


// This file is compiled without /Zc:__cplusplus, so __cplusplus is 199711L. It compiles
// only if Poco/Config.h reads the standard from _MSVC_LANG.
#include "Poco/Net/SocketAddress.h"


// UNIX_LOCAL exists only with UNIX sockets enabled and afunix.h in the SDK.
#if !defined(POCO_NET_NO_UNIX_SOCKET) && __has_include(<afunix.h>)
static_assert(Poco::Net::SocketAddress::UNIX_LOCAL != Poco::Net::SocketAddress::IPv4);
#endif
