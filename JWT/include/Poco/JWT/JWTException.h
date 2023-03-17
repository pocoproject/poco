//
// JWTException.h
//
// Library: JWT
// Package: JWT
// Module:  JWTException
//
// Definition of the JWTException class.
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef JWT_JWTException_INCLUDED
#define JWT_JWTException_INCLUDED


#include "Poco/JWT/JWT.h"
#include "Poco/Exception.h"


namespace Poco {
namespace JWT {


POCO_DECLARE_EXCEPTION(JWT_API, JWTException, Poco::Exception)
POCO_DECLARE_EXCEPTION(JWT_API, ParseException, JWTException)
POCO_DECLARE_EXCEPTION(JWT_API, UnsupportedAlgorithmException, JWTException)
POCO_DECLARE_EXCEPTION(JWT_API, UnallowedAlgorithmException, JWTException)
POCO_DECLARE_EXCEPTION(JWT_API, SignatureException, JWTException)
POCO_DECLARE_EXCEPTION(JWT_API, SignatureVerificationException, SignatureException)
POCO_DECLARE_EXCEPTION(JWT_API, SignatureGenerationException, SignatureException)


} } // namespace Poco::JWT


#endif // JWT_JWTException_INCLUDED
