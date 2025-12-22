//
// Session.cpp
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  Session
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/Session.h"
#include <openssl/ssl.h>


namespace Poco {
namespace Net {


Session::Session(SSL_SESSION* pSession):
	_pSession(pSession)
{
}


Session::~Session()
{
	SSL_SESSION_free(_pSession);
}


bool Session::isResumable() const
{
#if OPENSSL_VERSION_NUMBER >= 0x10101000L
	return SSL_SESSION_is_resumable(_pSession) == 1;
#else
	return false;
#endif
}


} } // namespace Poco::Net
