//
// X509Store.h
//
//
// Library: Crypto
// Package: Certificate
// Module:  X509Store
//
// Definition of the X509Store class.
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_X509Store_INCLUDED
#define Crypto_X509Store_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/X509RevocationList.h"
#include "Poco/Crypto/X509Certificate.h"
#include <vector>


namespace Poco {
namespace Crypto {


class Crypto_API X509Store
{
	/// Utility class for a x509 store.
	/// Currently, only verification of a certificate chain are supported.
public:
	class VerifyResult
		/// Result class after verification of a certificate chain.
	{
		friend class X509Store;
	public:
		VerifyResult();
			/// Constructs VerifyResult object with default values.

		const X509Certificate& certificate() const;
			/// Returns the certificate which caused the error.

		int depth() const;
			/// Returns the depth of the error in the certificate chain.
			///
			/// This is a non-negative integer representing where in the
			/// certificate chain the error occurred. If it is zero it
			/// occurred in the end entity certificate, one if it is the
			/// certificate which signed the end entity certificate and so on.

		int code() const;
			/// Returns the error code.

		bool isOk() const;
			/// Returns true if verification of certification chain was successful.

		std::string string() const;
			/// Returns a human readable error string for verification error.

	private:
		VerifyResult(const X509Certificate& certificate, int depth, int code, std::string errorString);

		X509Certificate _certificate;
		int _depth;
		int _code;
		std::string _errorString;
	};

	X509Store();
		/// Constructs a default X509Store.

	X509Store(X509_STORE* store);
		/// Creates the X509Store from an existing
		/// OpenSSL store. Ownership is taken of the store.

	X509Store(const X509Store& store) = delete;
		// Cannot copy a store.

	X509Store(X509Store&& other);
		/// Move constructor.

	X509Store& operator =(X509Store other) = delete;
		// Cannot copy a store.

	static void swap(X509Store& first, X509Store& second);
		/// Exchanges the first store with the second store.

	~X509Store();
		/// Destroys the X509Store.

	void setX509Purpose(int purpose);
		/// Set OpenSSL verification purpose.

	void setX509VerifyFlags(unsigned long flags);
		/// Set OpenSSL verification flags.

	bool addCa(const X509Certificate& caCertificate);
		/// Add a certificate into x509 store.

	bool addChain(const X509Certificate::List& chain);
		/// Add a chain of certificate into x509 store.

	bool addCrl(const X509RevocationList& crl);
		/// Add a crl into x509 store.

	VerifyResult verifyCertificateChain(const X509Certificate& cert);
		/// The function attempts to discover and validate a certificate chain
		/// from a given x509 certificate and stored x509 certificates
		/// via addCa(), addChain() or addCrl() function.

	operator const X509_STORE*() const;
		/// Returns const pointer to the OpenSSL X509_STORE structure.

	operator X509_STORE*();
		/// Returns pointer to the OpenSSL X509_STORE structure.

private:
	X509_STORE* _pStore;
	int _purpose;
};


//
// inlines
//
inline const X509Certificate& X509Store::VerifyResult::certificate() const
{
	return _certificate;
}


inline int X509Store::VerifyResult::depth() const
{
	return _depth;
}


inline int X509Store::VerifyResult::code() const
{
	return _code;
}


inline bool X509Store::VerifyResult::isOk() const
{
	return _code == 0;
}


inline std::string X509Store::VerifyResult::string() const
{
	return _errorString;
}


inline X509Store::operator X509_STORE *()
{
	return _pStore;
}


inline X509Store::operator const X509_STORE *() const
{
	return _pStore;
}


} } // namespace Poco::Crypto


#endif // Crypto_X509Store_INCLUDED
