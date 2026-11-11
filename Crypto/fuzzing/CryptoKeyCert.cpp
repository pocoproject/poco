//
// CryptoKeyCert.cpp
//
// Fuzzer for the Poco::Crypto key and certificate parsing surface.
//
// Threat model:
// - Untrusted input: PEM/DER key and certificate bytes — what an attacker
//   controls in client-supplied certificates, JWKS-style key material,
//   PKCS#7 bundles and any configuration that loads crypto material from
//   an untrusted source.
// - Trust boundary: X509Certificate's stream constructor (PEM), the
//   RSAKey/ECKey stream constructors (PEM public/private keys) and the
//   signature verification path.
// - As of the 2026-11-10 coverage report this surface sits at 0% project-wide
//   (EVPPKey.cpp 410, RSAKeyImpl.cpp 335, ECKeyImpl.cpp 175,
//   ECDSADigestEngine.cpp 115, RSADigestEngine.cpp 51 dark lines, plus the
//   X509Certificate implementation); the jwt fuzzer only initializes crypto
//   and never loads a key. No existing harness reaches it (verified via the
//   reports-by-target union).
// - Key generation is deliberately NOT attacker-influenced: a small RSA and
//   an EC key are generated once at first invocation (lazy static) and only
//   feed the sign/verify direction. All attacker bytes go into the parsers.
//   This keeps the fuzzer fast and points it at parsing/verification bugs,
//   not at keygen.
// - What counts as a bug: memory-safety violations in PEM/DER parsing or
//   name extraction (ASan/UBSan findings), crashes in signature padding or
//   ECDSA r/s handling on verification.
//

#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/DigestEngine.h"
#include "Poco/Crypto/ECDSADigestEngine.h"
#include "Poco/Crypto/ECKey.h"
#include "Poco/Crypto/EVPPKey.h"
#include "Poco/Crypto/RSAKey.h"
#include "Poco/Crypto/RSADigestEngine.h"
#include "Poco/Crypto/X509Certificate.h"

#include <set>
#include <sstream>
#include <string>

using namespace Poco;
using namespace Poco::Crypto;


namespace
{

RSAKey& rsaKey()
{
	static RSAKey key(RSAKey::KL_1024, RSAKey::EXP_SMALL);
	return key;
}

ECKey& ecKey()
{
	static ECKey key("prime256v1");
	return key;
}

template <typename F>
void ignore(const F& func)
{
	try
	{
		func();
	}
	catch (const std::exception&)
	{
	}
}


void fuzzCertificate(const std::string& pem)
{
	std::istringstream in(pem);

	// X509Certificate is not reference-counted, so the parsed certificate is
	// kept on the stack; a garbage parse throws and the whole block is skipped.
	ignore([&]
	{
		X509Certificate cert(in);

		{
			[[maybe_unused]] long v = cert.version();
		}
		{
			[[maybe_unused]] const std::string& v = cert.serialNumber();
		}
		{
			[[maybe_unused]] const std::string& v = cert.issuerName();
		}
		{
			[[maybe_unused]] const std::string& v = cert.subjectName();
		}
		{
			[[maybe_unused]] std::string v = cert.commonName();
		}
		{
			[[maybe_unused]] std::string v = cert.issuerName(X509Certificate::NID_COMMON_NAME);
		}
		{
			[[maybe_unused]] std::string v = cert.subjectName(X509Certificate::NID_PKCS9_EMAIL_ADDRESS);
		}
		{
			[[maybe_unused]] std::string v = cert.subjectName(X509Certificate::NID_SERIAL_NUMBER);
		}
		{
			[[maybe_unused]] DateTime v = cert.validFrom();
		}
		{
			[[maybe_unused]] DateTime v = cert.expiresOn();
		}
		{
			[[maybe_unused]] std::string v = cert.signatureAlgorithm();
		}
		{
			std::string cn;
			std::set<std::string> domains;
			cert.extractNames(cn, domains);
		}
		{
			[[maybe_unused]] Poco::DigestEngine::Digest d = cert.fingerprint("SHA1");
		}
		{
			[[maybe_unused]] Poco::DigestEngine::Digest d = cert.fingerprint("SHA256");
		}
		{
			[[maybe_unused]] bool v = cert.issuedBy(cert);
		}
		{
			[[maybe_unused]] bool v = cert.equals(cert);
		}
		{
			std::ostringstream out;
			cert.save(out);
			cert.print(out);
		}

		// Extract the public key from whatever parsed.
		{
			EVPPKey k(cert);
			RSAKey rk(k);
		}
		{
			RSAKey rk(cert);
			[[maybe_unused]] RSAKeyImpl::ByteVec m = rk.modulus();
			[[maybe_unused]] RSAKeyImpl::ByteVec e = rk.encryptionExponent();
		}
		{
			ECKey ek(cert);
		}
	});
}


void fuzzKeys(const std::string& pem)
{
	std::istringstream pub(pem);
	ignore([&]
	{
		RSAKey k(&pub);
		[[maybe_unused]] RSAKeyImpl::ByteVec m = k.modulus();
	});

	std::istringstream pubPriv(pem);
	ignore([&]
	{
		RSAKey k(&pubPriv, &pubPriv, "");
		[[maybe_unused]] RSAKeyImpl::ByteVec d = k.decryptionExponent();
	});

	std::istringstream ecPub(pem);
	ignore([&]
	{
		ECKey k(&ecPub);
	});

	std::istringstream ecPubPriv(pem);
	ignore([&]
	{
		ECKey k(&ecPubPriv, &ecPubPriv, "");
	});
}


void fuzzSignVerify(const std::string& message)
{
	// Sign with harness-owned keys, then verify the signature and a mutated
	// one; attacker bytes only choose the message.
	ignore([&]
	{
		RSADigestEngine rsa(rsaKey(), "SHA256");
		rsa.update(message);
		Poco::DigestEngine::Digest sig = rsa.signature();
		[[maybe_unused]] bool ok = rsa.verify(sig);
		if (!sig.empty())
		{
			Poco::DigestEngine::Digest bad = sig;
			bad[0] ^= 0x01;
			[[maybe_unused]] bool rejected = rsa.verify(bad);
		}
	});

	ignore([&]
	{
		ECDSADigestEngine ecdsa(ecKey(), "SHA256");
		ecdsa.update(message);
		Poco::DigestEngine::Digest sig = ecdsa.signature();
		[[maybe_unused]] bool ok = ecdsa.verify(sig);
		if (!sig.empty())
		{
			Poco::DigestEngine::Digest bad = sig;
			bad[0] ^= 0x02;
			[[maybe_unused]] bool rejected = ecdsa.verify(bad);
		}
	});

	// Truncated/corrupted signature blobs straight from attacker bytes.
	ignore([&]
	{
		Poco::DigestEngine::Digest sig(message.begin(), message.end());
		RSADigestEngine rsa(rsaKey(), "SHA512");
		rsa.update(message);
		[[maybe_unused]] bool v = rsa.verify(sig);
	});
}


void fuzzPlainDigest(const std::string& message)
{
	ignore([&]
	{
		Poco::Crypto::DigestEngine de("SHA256");
		de.update(message);
		[[maybe_unused]] Poco::DigestEngine::Digest d = de.digest();
	});
}


} // namespace


extern "C" int LLVMFuzzerInitialize(int* argc, char*** argv)
{
	Poco::Crypto::initializeCrypto();
	return 0;
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	// Input layout: [op:1][payload] — the payload doubles as the untrusted
	// PEM/DER bytes and the signed message.
	if (size < 1) return 0;

	const std::uint8_t op = data[0];
	std::string payload(reinterpret_cast<const char*>(data) + 1, size - 1);

	const bool runAll = op == 0;

	if (runAll || op & 0x01)
		fuzzCertificate(payload);

	if (runAll || op & 0x02)
		fuzzKeys(payload);

	if (runAll || op & 0x04)
		fuzzSignVerify(payload);

	if (runAll || op & 0x08)
		fuzzPlainDigest(payload);

	return 0;
}
