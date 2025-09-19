//
// JWT.cppm
//
// C++ module file
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_JWT
#include "Poco/JWT/JWTException.h"
#include "Poco/JWT/JWT.h"
#include "Poco/JWT/Serializer.h"
#include "Poco/JWT/Signer.h"
#include "Poco/JWT/Token.h"
#endif

export module Poco.JWT;

export namespace Poco::JWT {
	#ifdef ENABLE_JWT
	using Poco::JWT::JWTException;
	using Poco::JWT::ParseException;
	using Poco::JWT::Serializer;
	using Poco::JWT::SignatureException;
	using Poco::JWT::SignatureGenerationException;
	using Poco::JWT::SignatureVerificationException;
	using Poco::JWT::Signer;
	using Poco::JWT::Token;
	using Poco::JWT::UnallowedAlgorithmException;
	using Poco::JWT::UnsupportedAlgorithmException;
	#endif
}
