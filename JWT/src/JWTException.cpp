//
// JWTException.cpp
//
// Library: JWT
// Package: JWT
// Module:  JWTException
//
// Copyright (c) 2019, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/JWT/JWTException.h"
#include <typeinfo>


using Poco::Exception;


namespace Poco {
namespace JWT {


POCO_IMPLEMENT_EXCEPTION(JWTException, Exception, "JWT Exception")
POCO_IMPLEMENT_EXCEPTION(ParseException, JWTException, "JWT parsing failed")
POCO_IMPLEMENT_EXCEPTION(UnsupportedAlgorithmException, JWTException, "Unsupported signing algorithm")
POCO_IMPLEMENT_EXCEPTION(UnallowedAlgorithmException, JWTException, "Unallowed signing algorithm")
POCO_IMPLEMENT_EXCEPTION(SignatureException, JWTException, "JWT Signature Exception")
POCO_IMPLEMENT_EXCEPTION(SignatureVerificationException, SignatureException, "JWT signature verification failed")
POCO_IMPLEMENT_EXCEPTION(SignatureGenerationException, SignatureException, "JWT signature generation failed")


} } // namespace Poco::JWT
