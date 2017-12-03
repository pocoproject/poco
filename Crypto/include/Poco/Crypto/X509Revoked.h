//
// X509RevocationList.h
//
//
// Library: Crypto
// Package: Certificate
// Module:  X509Revoked
//
// Definiton of the X509Revoked class
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_X509Revoked_INCLUDED
#define Crypto_X509Revoked_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/X509Extension.h"
#include "Poco/DateTime.h"
#include <vector>


namespace Poco {
namespace Crypto {


class Crypto_API X509Revoked
	/// This class represents a X509 revoked certificate
{
public:
	typedef std::vector<X509Revoked> List;

	enum ReasonCode
		/// Reason codes for the certificate revocation.
		// From RFC 5280:
		// CRLReason ::= ENUMERATED {
		//	unspecified             (0),
		//	keyCompromise           (1),
		//	cACompromise            (2),
		//	affiliationChanged      (3),
		//	superseded              (4),
		//	cessationOfOperation    (5),
		//	certificateHold         (6),
		//		 -- value 7 is not used
		//	removeFromCRL           (8),
		//	privilegeWithdrawn      (9),
		//	aACompromise           (10) }
	{
		UNSPECIFIED = 0,
		KEY_COMPROMISE,
		CA_COMPROMISE,
		AFFILIATION_CHANGED,
		SUPERSEDED,
		CESSATION_OF_OPERATION,
		REMOVE_FROM_CRL = 8
	};


	X509Revoked();
		/// Constructs the X509Revoked object

	explicit X509Revoked(X509_REVOKED* pRevoked);
		/// Creates the X509Revoked from an existing
		/// OpenSSL revoked. Ownership is taken of
		/// the revoked.

	X509Revoked(const X509Revoked& revoked);
		/// Constructs the revoked by copying another one.

	X509Revoked(X509Revoked&& other);
		/// Move constructor

	X509Revoked& operator = (X509Revoked revoked);
		/// Assigns a revoked.

	static void swap(X509Revoked& first, X509Revoked& second);
		/// Exchanges the revoked with another one.

	virtual ~X509Revoked();
		/// Destroys the X509Revoked object

	void setSerialNumber(long serial);
		/// Set revoked certificate serial number

	void setSerialNumber(const std::string& serial);
		/// Set revoked certificate serial number

	std::string serialNumber() const;
		/// Get serial number of revoked certificate

	void setRevocationDate(Poco::DateTime date);
		/// Set the date the certificate is revoked from.

	Poco::DateTime getRevocationDate() const;
		/// Get the date the certificate is revoked from.

	bool setReasonCode(ReasonCode code);
		/// Set revocation reason code why the certificate is revoked.

	ReasonCode getReasonCode() const;
		/// Get revocation reason code why the certificate is revoked.

	void addExtension(const X509Extension& x509Extension);
		/// Add a specified extension at the end of the extension table.

	X509Extension::List findExtensionByNID(int nid);
		/// Search and get extension by openssl NID.

	operator const X509_REVOKED*() const;
		/// Returns const pointer to the OpenSSL X509_REVOKED structure.

	operator X509_REVOKED*();
		/// Returns pointer to the OpenSSL X509_REVOKED structure.

	static X509_REVOKED* duplicate(const X509_REVOKED* pFromRevoked, X509_REVOKED** pToRevoked);
		/// Duplicates pFromRevoked into *pToRevoked and returns
		/// the pointer to duplicated X509_REVOKED.

private:
	X509_REVOKED* _pRevoked;
};


//
// inlines
//


inline X509Revoked::operator const X509_REVOKED *() const
{
	return _pRevoked;
}


inline X509Revoked::operator X509_REVOKED *()
{
	return _pRevoked;
}


} } // namespace Poco::Crypto


#endif // Crypto_X509Revoked_INCLUDED
