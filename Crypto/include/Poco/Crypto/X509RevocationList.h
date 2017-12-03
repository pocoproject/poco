//
// X509RevocationList.h
//
//
// Library: Crypto
// Package: Certificate
// Module:  X509RevocationList
//
// Definiton of the X509RevocationList class.
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Crypto_X509RevocationList_INCLUDED
#define Crypto_X509RevocationList_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/EVPPKey.h"
#include "Poco/Crypto/X509Extension.h"
#include "Poco/Crypto/X509Revoked.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/DateTime.h"
#include "Poco/SharedPtr.h"
#include <openssl/x509.h>
#include <set>
#include <vector>
#include <istream>


namespace Poco {
namespace Crypto {


class Crypto_API X509RevocationList
	/// This class represents a X509 certificate revocation list.
{
public:
	enum NID
		/// Name identifier for extracting information from
		/// a certificate revocation list subject's or issuer's distinguished name.
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

	X509RevocationList();
		/// Creates the X509RevocationList object
	
	explicit X509RevocationList(std::istream& istr);
		/// Creates the X509RevocationList object by reading
		/// a certificate revocation list in PEM format from a stream.

	explicit X509RevocationList(const std::string& path);
		/// Creates the X509RevocationList object by reading
		/// a certificate revocation list in PEM format from a file.

	explicit X509RevocationList(X509_CRL* pCert);
		/// Creates the X509RevocationList from an existing
		/// OpenSSL certificate revocation list. Ownership is taken of
		/// the certificate revocation list.

	X509RevocationList(X509_CRL* pCert, bool shared);
		/// Creates the X509RevocationList from an existing
		/// OpenSSL certificate revocation list. Ownership is taken of
		/// the certificate revocation list. If shared is true, the
		/// certificate revocation list's reference count is incremented.

	X509RevocationList(const X509RevocationList& cert);
		/// Creates the certificate revocation list by copying another one.

	X509RevocationList(X509RevocationList&& other);
		/// Move constructor

	X509RevocationList& operator =(X509RevocationList other);
		/// Assigns a certificate revocation list.
 
	static void swap(X509RevocationList& first, X509RevocationList& second);
		/// Exchanges the certificate revocation list with another one.

	~X509RevocationList();
		/// Destroys the X509RevocationList.

	long version() const;
		/// Get the certificate revocation list version

	void addIssuer(NID nid, const std::string& name);
		/// Set certificate revocation list issuer name specified by the given NID (name identifier)

	const std::string& issuerName() const;
		/// Returns the certificate revocation list issuer's distinguished name.
		
	std::string issuerName(NID nid) const;
		/// Extracts the information specified by the given
		/// NID (name identifier) from the certificate revocation list issuer's
		/// distinguished name.
		
	void extractNames(std::set<std::string>& domainNames) const;
		/// Extracts the common name and the alias domain names from the
		/// certificate revocation list.
		
	void setLastUpdate(Poco::DateTime dateTime);
		/// Set the date and time the certificate revocation list last update.

	Poco::DateTime lastUpdate() const;
		/// Returns the date and time the certificate revocation list last update.
		
	void setNextUpdate(Poco::DateTime expires);
		/// Set the date and time the certificate revocation list next update.

	Poco::DateTime nextUpdate() const;
		/// Returns the date and time the certificate revocation list next update.
		
	void addExtension(const X509Extension& x509Extension);
		/// Add a specified extension at the and of the extension table.

	void addRevoked(const X509Revoked& revoked);
		/// Appends revoked entry to revocation list.

	X509Revoked findRevokedBySerialNumber(long serialNumber);
		/// Attempts to find a revoked entry in revocation list for serial number.

	X509Revoked findRevokedByCertificate(const X509Certificate& certificate);
		/// Attempts to find a revoked entry in revocation list for using the serial number of certificate.

	X509Revoked::List getAllRevoked() const;
		/// Returns a vector of all revoked entries in revocation list.

	X509Extension::List findExtensionByNID(int nid);
		/// Search and get extension by openssl NID.

	void save(std::ostream& stream) const;
		/// Writes the certificate revocation list to the given stream.
		/// The certificate revocation list is written in PEM format.

	void save(const std::string& path) const;
		/// Writes the certificate revocation list to the file given by path.
		/// The certificate revocation list is written in PEM format.
		
	bool verify(const EVPPKey& key) const;
		/// Returns true if verification against the key
		/// was successfull, false otherwise.

	bool sign(const EVPPKey& pkey, const std::string& mdstr = "default");
		/// Sign the certificate revocation list

	bool equals(const X509RevocationList& otherCertificate) const;
		/// Checks whether the certificate revocation list is equal to
		/// the other certificate revocation list, by comparing the hashes
		/// of both certificates.
		///
		/// Returns true if both certificates are identical,
		/// otherwise false.

	operator const X509_CRL*() const;
		/// Returns const pointer to the OpenSSL X509_CRL structure.

	operator X509_CRL*();
		/// Returns pointer to the OpenSSL X509_CRL structure.

	static X509_CRL* duplicate(const X509_CRL* pFromCRL, X509_CRL** pToCRL);
		/// Duplicates pFromKey into *pToKey and returns
		/// the pointer to duplicated OpenSSL X509_CRL structure.

protected:
	void load(std::istream& stream);
		/// Loads the crl from the given stream. The
		/// crl must be in PEM format.
		
	void load(const std::string& path);
		/// Loads the crl from the given file. The
		/// crl must be in PEM format.

	void init();
		/// Extracts issuer name from the crl.
	
private:
	enum
	{
		NAME_BUFFER_SIZE = 256
	};
	
	std::string _issuerName;
	X509_CRL*       _pCrl;
};


//
// inlines
//
inline const std::string& X509RevocationList::issuerName() const
{
	return _issuerName;
}


inline X509RevocationList::operator const X509_CRL *() const
{
	return _pCrl;
}


inline X509RevocationList::operator X509_CRL *()
{
	return _pCrl;
}


} } // namespace Poco::Crypto


#endif // Crypto_X509RevocationList_INCLUDED
