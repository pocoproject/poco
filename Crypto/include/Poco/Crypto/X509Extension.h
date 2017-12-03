//
// X509Extension.h
//
//
// Library: Crypto
// Package: Certificate
// Module:  X509Extension
//
// Definition of the X509Extension class.
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_X509Extension_INCLUDE
#define Crypto_X509Extension_INCLUDE


#include "Poco/Crypto/Crypto.h"
#include <openssl/x509.h>
#include <string>
#include <vector>


namespace Poco {
namespace Crypto {


class Crypto_API X509Extension
{
public:
	typedef std::vector<X509Extension> List;

	enum BASIC_CONTRAINTS
		/// Are used to indicate wehether the certificate belongs to a CA.
	{
		CRITICAL_CA_TRUE,
		CRITICAL_CA_FALSE,
		CA_TRUE,
		CA_FALSE
	};

	static X509Extension createWithBasicConstraints(BASIC_CONTRAINTS type);
		/// Create the X509Extension with a given basic constraints.

	static X509Extension create(int nid, const std::string& value);
		/// Create the X509Extension from the given OpenSSL NID and value. Example: create(NID_basic_constraints, "critical, CA:TRUE")

	static X509Extension create(X509V3_CTX *ctx, int nid, const std::string &value);
		/// Create the X509Extension from the given OpenSSL x509v3 context, openssl NID and value.

	static X509Extension create(int nid, bool critical, void* data, size_t lenght);
		/// Create the X509Extension from the given OpenSSL NID and pointer to value.

	X509Extension(X509_EXTENSION* pExt);
		/// Create the X509Extension from an existing OpenSSL X509_EXTENSION pointer.

	X509Extension(const X509Extension& other);
		/// Creates the extension by copying another one.

	X509Extension& operator = (X509Extension ext);
		/// Assigns a extension.

	X509Extension(X509Extension&& other);
		/// Move constructor

	static void swap(X509Extension& first, X509Extension& second);
		/// Exchanges the certificate with another one.

	~X509Extension();
		/// Destroys the X509Extension.

	bool isCritical() const;
		/// Is the x509 extension critical flag set.

	int nid() const;
		/// Returns the OpenSSL NID.

	const unsigned char* data() const;
		/// Returns a pointer to the x509 extension raw value.
		///
		/// In general it cannot be assumed that the data returned by this function is null
		/// terminated or does not contain embedded nulls. Use this function together with
		/// the size() function to get the length of data.

	size_t size() const;
		/// Returns the length of data.
		///
		/// See data() function

	std::string str() const;
		/// Returns x509 extension value as string.

	operator const X509_EXTENSION*() const;
		/// Returns pointer to the OpenSSL X509_EXTENSION structure.

	operator X509_EXTENSION*();
		/// Returns pointer to the OpenSSL X509_EXTENSION structure.

	static X509_EXTENSION* duplicate(const X509_EXTENSION* pFromExtension, X509_EXTENSION** pToExtension);
		/// Duplicates pFromExtension into *pToExtension and returns
		/// the pointer to duplicated OpenSSL X509_EXTENSION structure.

private:
	X509_EXTENSION* _pExt;
};


//
// inlines
//


inline X509Extension::operator const X509_EXTENSION *() const
{
	return _pExt;
}


inline X509Extension::operator X509_EXTENSION *()
{
	return _pExt;
}


} } // namespace Poco::Crypto


#endif // Crypto_X509Extension_INCLUDE
