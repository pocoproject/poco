//
// X509Certificate.h
//
// $Id: //poco/1.4/Crypto/include/Poco/Crypto/X509Certificate.h#2 $
//
// Library: Crypto
// Package: Certificate
// Module:  X509Certificate
//
// Definition of the X509Certificate class.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef NetSSL_X509Certificate_INCLUDED
#define NetSSL_X509Certificate_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/DateTime.h"
#include "Poco/SharedPtr.h"
#include <set>
#include <wincrypt.h>


namespace Poco {
namespace Net {


class NetSSL_Win_API X509Certificate
	/// This class represents a X509 Certificate.
{
public:
	enum NID
		/// Name identifier for extracting information from
		/// a certificate subject's or issuer's distinguished name.
	{
		NID_COMMON_NAME,
		NID_COUNTRY,
		NID_LOCALITY_NAME,
		NID_STATE_OR_PROVINCE,
		NID_ORGANIZATION_NAME,
		NID_ORGANIZATION_UNIT_NAME	
	};
	
	explicit X509Certificate(std::istream& istr);
		/// Creates the X509Certificate object by reading
		/// a certificate in PEM format from a stream.

	explicit X509Certificate(const std::string& path);
		/// Creates the X509Certificate object by reading
		/// a certificate in PEM format from a file.

	explicit X509Certificate(PCCERT_CONTEXT pCert);
		/// Creates the X509Certificate from an existing
		/// WinCrypt certificate. Ownership is taken of 
		/// the certificate.

	X509Certificate(PCCERT_CONTEXT pCert, bool shared);
		/// Creates the X509Certificate from an existing
		/// WinCrypt certificate. Ownership is taken of 
		/// the certificate. If shared is true, the 
		/// certificate's reference count is incremented.

	X509Certificate(const X509Certificate& cert);
		/// Creates the certificate by copying another one.

	X509Certificate& operator = (const X509Certificate& cert);
		/// Assigns a certificate.
 
	void swap(X509Certificate& cert);
		/// Exchanges the certificate with another one.

	~X509Certificate();
		/// Destroys the X509Certificate.

	const std::string& issuerName() const;
		/// Returns the certificate issuer's distinguished name. 
		
	std::string issuerName(NID nid) const;
		/// Extracts the information specified by the given
		/// NID (name identifier) from the certificate issuer's
		/// distinguished name.
		
	const std::string& subjectName() const;
		/// Returns the certificate subject's distinguished name.

	std::string subjectName(NID nid) const;
		/// Extracts the information specified by the given
		/// NID (name identifier) from the certificate subject's
		/// distinguished name.
		
	std::string commonName() const;
		/// Returns the common name stored in the certificate
		/// subject's distinguished name.
		
	void extractNames(std::string& commonName, std::set<std::string>& domainNames) const;
		/// Extracts the common name and the alias domain names from the
		/// certificate.
		
	Poco::DateTime validFrom() const;
		/// Returns the date and time the certificate is valid from.
		
	Poco::DateTime expiresOn() const;
		/// Returns the date and time the certificate expires.
		
	void save(std::ostream& stream) const;
		/// Writes the certificate to the given stream.
		/// The certificate is written in PEM format.

	void save(const std::string& path) const;
		/// Writes the certificate to the file given by path.
		/// The certificate is written in PEM format.
		
	bool issuedBy(const X509Certificate& issuerCertificate) const;
		/// Checks whether the certificate has been issued by
		/// the issuer given by issuerCertificate. This can be
		/// used to validate a certificate chain.
		///
		/// Verifies if the certificate has been signed with the
		/// issuer's private key, using the public key from the issuer
		/// certificate.
		///
		/// Returns true if verification against the issuer certificate
		/// was successful, false otherwise.

	const PCCERT_CONTEXT system() const;
		/// Returns the underlying WinCrypt certificate.

protected:
	void load(std::istream& stream);
		/// Loads the certificate from the given stream. The
		/// certificate must be in PEM format.
		
	void load(const std::string& path);
		/// Loads the certificate from the given file. The
		/// certificate must be in PEM format.

	void init();
		/// Extracts issuer and subject name from the certificate.
	
	static void* nid2oid(NID nid);
		/// Returns the OID for the given NID.

private:
	enum
	{
		NAME_BUFFER_SIZE = 256
	};
	
	std::string _issuerName;
	std::string _subjectName;
	PCCERT_CONTEXT _pCert;
};


//
// inlines
//
inline const std::string& X509Certificate::issuerName() const
{
	return _issuerName;
}


inline const std::string& X509Certificate::subjectName() const
{
	return _subjectName;
}


inline const PCCERT_CONTEXT X509Certificate::system() const
{
	return _pCert;
}


} } // namespace Poco::Net


#endif // NetSSL_X509Certificate_INCLUDED
