//
// X509Request.h
//
//
// Library: Crypto
// Package: Certificate
// Module:  X509Request
//
// Definition of the X509Request class.
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_X509Request_INCLUDED
#define Crypto_X509Request_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/EVPPKey.h"
#include "Poco/Crypto/X509Extension.h"
#include "Poco/Crypto/OpenSSLInitializer.h"
#include "Poco/DateTime.h"
#include "Poco/SharedPtr.h"
#include <openssl/x509.h>
#include <set>
#include <vector>
#include <istream>


namespace Poco {
namespace Crypto {


class Crypto_API X509Request
	/// This class represents a X509 request certificate.
{
public:
	enum NID
		/// Name identifier for extracting information from
		/// a certificate subject's or issuer's distinguished name.
	{
		NID_COMMON_NAME = 13,
		NID_COUNTRY = 14,
		NID_LOCALITY_NAME = 15,
		NID_STATE_OR_PROVINCE = 16,
		NID_ORGANIZATION_NAME = 17,
		NID_ORGANIZATION_UNIT_NAME = 18,
		NID_PKCS9_EMAIL_ADDRESS = 48,
		NID_SERIAL_NUMBER = 105
	};

	X509Request();
		/// Creates the X509Request object
	
	explicit X509Request(std::istream& istr);
		/// Creates the X509Request object by reading
		/// a certificate in PEM format from a stream.

	explicit X509Request(const std::string& path);
		/// Creates the X509Request object by reading
		/// a certificate in PEM format from a file.

	explicit X509Request(X509_REQ* pReq);
		/// Creates the X509Request from an existing
		/// OpenSSL certificate. Ownership is taken of 
		/// the certificate.

#if OPENSSL_VERSION_NUMBER <= 0x10100000L
	X509Request(X509_REQ* pReq, bool shared);
		/// Creates the X509Request from an existing
		/// OpenSSL certificate. Ownership is taken of 
		/// the certificate. If shared is true, the 
		/// certificate's reference count is incremented.
#endif

	X509Request(const X509Request& other);
		/// Creates the certificate by copying another one.

	X509Request(X509Request&& other);
		/// Move constructor

	X509Request& operator = (X509Request req);
		/// Assigns a certificate.
 
	static void swap(X509Request& first, X509Request& second);
		/// Exchanges the certificate with another one.

	~X509Request();
		/// Destroys the X509Request.

	long version() const;
		/// Get the certificate version

	void addSubject(NID nid, const std::string& name);
		/// Set the information specified by the given
		/// NID (name identifier) from the certificate subject's
		/// distinguished name.

	const std::string& subjectName() const;
		/// Returns the certificate subject's distinguished name.

	std::string subjectName(NID nid) const;
		/// Extracts the information specified by the given
		/// NID (name identifier) from the certificate subject's
		/// distinguished name.
		
	void setCommonName(const std::string& cn);
		/// Set the common name stored in the certificate
		/// subject's distinguished name.

	std::string commonName() const;
		/// Returns the common name stored in the certificate
		/// subject's distinguished name.
		
	void setPublicKey(const EVPPKey& pkey);
		/// Set public key

	EVPPKey publicKey() const;
		/// Return public key

    void addExtension(const X509Extension& x509Extension);
        /// Add a specified extension at the and of the extension table.

	void setExtensions(const X509Extension::List& x509Extensions);
		/// Add a specified extensions.

	X509Extension::List extensions();
		/// Get all requested extension.

	void save(std::ostream& stream) const;
		/// Writes the certificate to the given stream.
		/// The certificate is written in PEM format.

	void save(const std::string& path) const;
		/// Writes the certificate to the file given by path.
		/// The certificate is written in PEM format.
		
	bool issuedBy(const X509Request& issuerCertificate) const;
		/// Checks whether the certificate has been issued by
		/// the issuer given by issuerCertificate. This can be
		/// used to validate a certificate chain.
		///
		/// Verifies if the certificate has been signed with the
		/// issuer's private key, using the public key from the issuer
		/// certificate.
		///
		/// Returns true if verification against the issuer certificate
		/// was successfull, false otherwise.

	int sign(const EVPPKey& pkey, const std::string& mdstr = "default");
		/// Sign the certificate
		///
		/// Return the size of the signature in bytes for success and
		/// zero for failure.

	void print(std::ostream& out) const;
		/// Prints the request information to ostream.

	operator const X509_REQ*() const;
		/// Returns const pointer to the OpenSSL X509_REQ structure.

	operator X509_REQ*();
		/// Returns pointer to the OpenSSL X509_REQ structure.

	static X509_REQ* duplicate(const X509_REQ* pFromRequest, X509_REQ** pToRequest);
		/// Duplicates pFromRequest into *pToRequest and returns
		/// the pointer to duplicated OpenSSL X509_REQ structure.

protected:
	void load(std::istream& stream);
		/// Loads the certificate from the given stream. The
		/// certificate must be in PEM format.
		
	void load(const std::string& path);
		/// Loads the certificate from the given file. The
		/// certificate must be in PEM format.

	void init();
		/// Extracts issuer and subject name from the certificate.
	
private:
	enum
	{
		NAME_BUFFER_SIZE = 256
	};
	
	std::string _subjectName;
	X509_REQ*   _pRequest;
};


//
// inlines
//


inline const std::string& X509Request::subjectName() const
{
	return _subjectName;
}


inline X509Request::operator const X509_REQ *() const
{
	return _pRequest;
}


inline X509Request::operator X509_REQ *()
{
	return _pRequest;
}


} } // namespace Poco::Crypto


#endif // Crypto_X509Request_INCLUDED
