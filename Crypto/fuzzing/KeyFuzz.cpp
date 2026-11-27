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
#include <cstddef>
#include <cstdint>
#include <sstream>
#include <string>


using namespace Poco;
using namespace Poco::Crypto;


namespace
{

constexpr std::size_t MAX_INPUT_SIZE = 16384;


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
	if (size > MAX_INPUT_SIZE) return 0;

	[[maybe_unused]] static const bool initialized = []
	{
		initializeCrypto();
		return true;
	}();

	const std::string pem(reinterpret_cast<const char*>(data), size);
	if (pem.find("ENCRYPTED") != std::string::npos) return 0;

	attempt([&]
	{
		std::istringstream in(pem);
		EVPPKey key(&in, nullptr);
	});
	attempt([&]
	{
		std::istringstream in(pem);
		EVPPKey key(nullptr, &in, "");
	});
	attempt([&]
	{
		std::istringstream in(pem);
		RSAKey key(nullptr, &in, "");
	});
	attempt([&]
	{
		std::istringstream in(pem);
		RSAKey key(&in);
	});
	attempt([&]
	{
		std::istringstream in(pem);
		ECKey key(&in);
	});

	return 0;
}
