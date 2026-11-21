//
// KeyFuzz.cpp
//
// Feeds one PEM key file into EVPPKey, RSAKey and ECKey stream constructors.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/CryptoException.h"
#include "Poco/Crypto/ECKey.h"
#include "Poco/Crypto/EVPPKey.h"
#include "Poco/Crypto/RSAKey.h"
#include "Poco/Exception.h"
#include <cstdint>
#include <sstream>
#include <string>


using namespace Poco;
using namespace Poco::Crypto;


namespace
{
	template <typename F>
	void attempt(F f)
		/// Runs f and passes over OpenSSLException, IOException and
		/// NotImplementedException. Anything else is a finding.
	{
		try
		{
			f();
		}
		catch (const OpenSSLException&)
		{
		}
		catch (const IOException&)
		{
		}
		catch (const NotImplementedException&)
		{
		}
	}
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	static const bool initialized = []
	{
		initializeCrypto();
		return true;
	}();
	(void) initialized;

	if (size > 16 * 1024) return 0;

	const std::string pem(reinterpret_cast<const char*>(data), size);
	if (pem.find("ENCRYPTED") != std::string::npos) return 0;

	attempt([&]
	{
		std::istringstream in(pem);
		EVPPKey key(&in, nullptr);
		(void) key;
	});
	attempt([&]
	{
		std::istringstream in(pem);
		EVPPKey key(nullptr, &in, "");
		(void) key;
	});
	attempt([&]
	{
		std::istringstream in(pem);
		RSAKey key(&in);
		(void) key;
	});
	attempt([&]
	{
		std::istringstream in(pem);
		ECKey key(&in);
		(void) key;
	});

	return 0;
}
