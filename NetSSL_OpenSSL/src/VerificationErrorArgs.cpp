//
// VerificationErrorArgs.cpp
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  VerificationErrorArgs
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include <utility>

#include "Poco/Net/VerificationErrorArgs.h"


namespace Poco {
namespace Net {


VerificationErrorArgs::VerificationErrorArgs(Poco::Net::Context::Ptr pContext, X509Certificate  cert, int errDepth, int errNum, std::string  errMsg):
	_pContext(std::move(pContext)),
	_cert(std::move(cert)),
	_errorDepth(errDepth),
	_errorNumber(errNum),
	_errorMessage(std::move(errMsg)),
	_ignoreError(false)
{
}


VerificationErrorArgs::~VerificationErrorArgs()
{
}


} } // namespace Poco::Net
