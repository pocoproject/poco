//
// VerificationArgs.cpp
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  VerificationArgs
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/VerificationArgs.h"


namespace Poco {
namespace Net {


VerificationArgs::VerificationArgs(const X509Certificate& cert, const X509Certificate& certToCheck, int depth, int code, const std::string& msg):
	_cert(cert),
	_certToCheck(certToCheck),
	_depth(depth),
	_code(code),
	_message(msg),
	_error(false)
{
}


VerificationArgs::~VerificationArgs()
{
}


} } // namespace Poco::Net
