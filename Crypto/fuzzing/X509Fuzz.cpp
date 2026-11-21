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
#include <cstdint>
#include <cstdlib>
#include <set>
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
		pem << "\n-----END CERTIFICATE-----\n";
		return pem.str();
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

	const std::string der(reinterpret_cast<const char*>(data), size);
	const std::string pem = derToPem(der);

	attempt([&]
	{
		std::istringstream in(pem);
		X509Certificate cert(in);

		attempt([&] { (void) cert.version(); });
		attempt([&] { (void) cert.serialNumber(); });
		attempt([&] { (void) cert.issuerName(); });
		attempt([&] { (void) cert.subjectName(); });
		attempt([&] { (void) cert.commonName(); });
		attempt([&] { (void) cert.issuerName(X509Certificate::NID_COMMON_NAME); });
		attempt([&] { (void) cert.subjectName(X509Certificate::NID_PKCS9_EMAIL_ADDRESS); });
		attempt([&] { (void) cert.subjectName(X509Certificate::NID_SERIAL_NUMBER); });

		try
		{
			(void) cert.validFrom();
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
		catch (const SyntaxException&)
		{
		}

		try
		{
			(void) cert.expiresOn();
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
		catch (const SyntaxException&)
		{
		}

		try
		{
			(void) cert.signatureAlgorithm();
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
		catch (const OpenSSLException&)
		{
		}
		catch (const IOException&)
		{
		}
		catch (const NotImplementedException&)
		{
		}
		catch (const InvalidArgumentException&)
		{
		}

		attempt([&] { (void) cert.equals(cert); });

		attempt([&]
		{
			EVPPKey key(cert);
			(void) key;
		});
		attempt([&]
		{
			RSAKey key(cert);
			(void) key;
		});
		attempt([&]
		{
			ECKey key(cert);
			(void) key;
		});

		try
		{
			std::ostringstream out;
			cert.save(out);
			std::istringstream again(out.str());
			X509Certificate copy(again);
			if (!cert.equals(copy))
				std::abort();
		}
		catch (const OpenSSLException&)
		{
			std::abort();
		}
		catch (const IOException&)
		{
			std::abort();
		}
		catch (const NotImplementedException&)
		{
			std::abort();
		}
	});

	return 0;
}
