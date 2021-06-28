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


#include "Poco/Net/VerificationErrorArgs.h"


namespace Poco {
namespace Net {


VerificationErrorArgs::VerificationErrorArgs(Poco::Net::Context::Ptr pContext, const X509Certificate& cert, int errDepth, int errNum, const std::string& errMsg):
	_pContext(pContext),
	_cert(cert),
	_errorDepth(errDepth),
	_errorNumber(errNum),
	_errorMessage(errMsg),
	_ignoreError(false)
{
}


VerificationErrorArgs::~VerificationErrorArgs()
{
}


} } // namespace Poco::Net
