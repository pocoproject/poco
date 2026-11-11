//
// X509Fuzz.cpp
//
// Feeds one DER X.509 certificate into Poco::Crypto::X509Certificate via an
// in-memory PEM wrapper, then exercises accessors and key extraction.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Base64Encoder.h"
#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/CryptoException.h"
#include "Poco/Crypto/ECKey.h"
#include "Poco/Crypto/EVPPKey.h"
#include "Poco/Crypto/RSAKey.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/Exception.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <set>
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


	std::string derToPem(const std::string& der)
		/// Wraps DER certificate bytes as a PEM CERTIFICATE block in memory.
	{
		std::ostringstream pem;
		pem << "-----BEGIN CERTIFICATE-----\n";
		{
			Base64Encoder enc(pem);
			enc.write(der.data(), static_cast<std::streamsize>(der.size()));
			enc.close();
		}
		std::string encoded = pem.str();
		// The encoder ends its output with a line break exactly when the
		// input length is a multiple of the line length; write the break in
		// front of the END line only when it is missing, or an empty line
		// would make OpenSSL reject the block.
		if (encoded.empty() || encoded.back() != '\n')
			encoded += '\n';
		encoded += "-----END CERTIFICATE-----\n";
		return encoded;
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

	const std::string der(reinterpret_cast<const char*>(data), size);
	const std::string pem = derToPem(der);

	attempt([&]
	{
		std::istringstream in(pem);
		X509Certificate cert(in);

		(void) cert.version();
		(void) cert.serialNumber();
		(void) cert.issuerName();
		(void) cert.subjectName();
		(void) cert.commonName();
		(void) cert.issuerName(X509Certificate::NID_COMMON_NAME);
		(void) cert.subjectName(X509Certificate::NID_PKCS9_EMAIL_ADDRESS);
		(void) cert.subjectName(X509Certificate::NID_SERIAL_NUMBER);

		try
		{
			(void) cert.validFrom();
		}
		catch (const SyntaxException&)
		{
		}
		catch (const NotImplementedException&)
		{
		}

		try
		{
			(void) cert.expiresOn();
		}
		catch (const SyntaxException&)
		{
		}
		catch (const NotImplementedException&)
		{
		}

		try
		{
			(void) cert.signatureAlgorithm();
		}
		catch (const OpenSSLException&)
		{
		}
		catch (const NotFoundException&)
		{
		}

		attempt([&]
		{
			std::string cn;
			std::set<std::string> domains;
			cert.extractNames(cn, domains);
		});
		attempt([&] { (void) cert.fingerprint("SHA1"); });
		attempt([&] { (void) cert.fingerprint("SHA256"); });

		try
		{
			(void) cert.issuedBy(cert);
		}
		catch (const InvalidArgumentException&)
		{
		}

		(void) cert.equals(cert);

		attempt([&]
		{
			EVPPKey key(cert);
		});
		attempt([&]
		{
			RSAKey key(cert);
		});
		attempt([&]
		{
			ECKey key(cert);
		});

		try
		{
			std::ostringstream out;
			cert.save(out);
			std::istringstream again(out.str());
			X509Certificate copy(again);
			if (!cert.equals(copy))
			{
				std::cerr << "X509Fuzz: saved certificate does not equal the original" << std::endl;
				std::abort();
			}
		}
		catch (const OpenSSLException&)
		{
			std::cerr << "X509Fuzz: save/restore round trip failed with OpenSSLException" << std::endl;
			std::abort();
		}
		catch (const IOException&)
		{
			std::cerr << "X509Fuzz: save/restore round trip failed with IOException" << std::endl;
			std::abort();
		}
		catch (const NotImplementedException&)
		{
			std::cerr << "X509Fuzz: save/restore round trip is not implemented" << std::endl;
			std::abort();
		}
	});

	return 0;
}
