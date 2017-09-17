//
// PKCS12Container.h
//
// $Id: //poco/1.4/Crypto/include/Poco/Crypto/PKCS12Container.h#2 $
//
// Library: Crypto
// Package: Certificate
// Module:  PKCS12Container
//
// Definition of the PKCS12Container class.
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_PKCS12Container_INCLUDED
#define Crypto_PKCS12Container_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/OpenSSLInitializer.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/Crypto/EVPPKey.h"
#include "Poco/Path.h"
#include <istream>
#include <memory>
#include <openssl/pkcs12.h>


namespace Poco {
namespace Crypto {


class Crypto_API PKCS12Container
	/// This class implements PKCS#12 container functionality.
{
public:
	typedef std::vector<X509Certificate> CAList;

	explicit PKCS12Container(std::istream& istr, const std::string& password = "");
		/// Creates the PKCS12Container object from a stream.

	explicit PKCS12Container(const std::string& str, const std::string& password = "");
		/// Creates the PKCS12Container object from a string.

	PKCS12Container(const PKCS12Container& cert);
		/// Copy constructor.

	PKCS12Container& operator = (const PKCS12Container& cert);
		/// Assignment operator.

	~PKCS12Container();
		/// Destroys the PKCS12Container.

	bool hasKey() const;
		/// Returns true if container contains the key.

	EVPPKey getKey() const;
		/// Return key as openssl EVP_PKEY wrapper object.

	bool hasX509Certificate() const;
		/// Returns true if container has X509 certificate.

	const X509Certificate& getX509Certificate() const;
		/// Returns the X509 certificate.
		/// Throws NotFoundException if there is no certificate.

	const CAList& getCACerts() const;
		/// Returns the list of CA certificates in this container.

	const std::string& getFriendlyName() const;
		/// Returns the friendly name of the certificate bag.

private:
	void load(PKCS12* pPKCS12, const std::string& password = "");

	typedef std::unique_ptr<X509Certificate> CertPtr;

	OpenSSLInitializer _openSSLInitializer;
	EVP_PKEY*          _pKey = 0;
	CertPtr            _pX509Cert;
	CAList             _caCertList;
	std::string        _pkcsFriendlyname;
};


//
// inlines
//

inline bool PKCS12Container::hasX509Certificate() const
{
	return (bool) _pX509Cert;
}


inline const X509Certificate& PKCS12Container::getX509Certificate() const
{
	if (!_pX509Cert)
		throw NotFoundException("PKCS12Container X509 certificate");
	return *_pX509Cert;
}


inline const std::string& PKCS12Container::getFriendlyName() const
{
	return _pkcsFriendlyname;
}


inline const PKCS12Container::CAList& PKCS12Container::getCACerts() const
{
	return _caCertList;
}


inline bool PKCS12Container::hasKey() const
{
	return _pKey != 0;
}


inline EVPPKey PKCS12Container::getKey() const
{
	return EVPPKey(_pKey);
}


} } // namespace Poco::Crypto


#endif // Crypto_PKCS12Container_INCLUDED
