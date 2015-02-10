//
// DigestEngine.h
//
// $Id$
//
// Library: Crypto_Win
// Package: Digest
// Module:  DigestEngine
//
// Definition of the DigestEngine class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_DigestEngine_INCLUDED
#define Crypto_DigestEngine_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/ServiceProvider.h"
#include "Poco/DigestEngine.h"


namespace Poco {
namespace Crypto {


class Crypto_Win_API DigestEngine: public Poco::DigestEngine
	/// This class implements a Poco::DigestEngine for all
	/// digest algorithms supported by Windows.
{
public:
	DigestEngine(const std::string& name);
		/// Creates a DigestEngine using the digest with the given name
		/// (e.g., "MD5", "SHA1", "SHA256", "SHA512", etc.).
		/// Algorithm names are not case sensitive.
		/// 
		/// The following digest algorithms are supported:
		///   - MD2
		///   - MD4
		///   - MD5
		///   - SHA1 (SHA-1)
		///   - SHA256 (SHA-256)
		///   - SHA384 (SHA-384)
		///   - SHA512 (SHA-512)
		/// 
		/// Throws a Poco::NotFoundException if no algorithm with the given name exists.
		
	~DigestEngine();
		/// Destroys the DigestEngine.
	
	const std::string& algorithm() const;
		/// Returns the name of the digest algorithm.
	
	// DigestEngine
	std::size_t digestLength() const;
	void reset();
	const Poco::DigestEngine::Digest& digest();

protected:
	void updateImpl(const void* data, std::size_t length);
	
private:
	ServiceProvider _sp;
	std::string _name;
	HCRYPTHASH _handle;
	Poco::DigestEngine::Digest _digest;
};


//
// inlines
//
inline const std::string& DigestEngine::algorithm() const
{
	return _name;
}


} } // namespace Poco::Crypto


#endif // Crypto_DigestEngine_INCLUDED
