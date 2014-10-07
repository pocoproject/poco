//
// X509Certificate.cpp
//
// $Id$
//
// Library: Crypto
// Package: Certificate
// Module:  X509Certificate
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/X509Certificate.h"
#include "Poco/Net/SSLException.h"
#include "Poco/StreamCopier.h"
#include "Poco/String.h"
#include "Poco/DateTimeParser.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/MemoryStream.h"
#include "Poco/Buffer.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/Format.h"
#include <sstream>


namespace Poco {
namespace Net {


X509Certificate::X509Certificate(const std::string& path):
	_pCert(0)
{
	importCertificate(path);
}


X509Certificate::X509Certificate(const std::string& certName, const std::string& certStoreName, bool useMachineStore):
	_pCert(0)
{
	loadCertificate(certName, certStoreName, useMachineStore);
}


X509Certificate::X509Certificate(PCCERT_CONTEXT pCert):
	_pCert(pCert)
{
	poco_check_ptr(_pCert);

	init();
}


X509Certificate::X509Certificate(const X509Certificate& cert):
	_issuerName(cert._issuerName),
	_subjectName(cert._subjectName),
	_pCert(cert._pCert)
{
	_pCert = CertDuplicateCertificateContext(_pCert);
}


X509Certificate::X509Certificate(PCCERT_CONTEXT pCert, bool shared):
	_pCert(pCert)
{
	poco_check_ptr(_pCert);
	
	if (shared)
	{
		_pCert = CertDuplicateCertificateContext(_pCert);
	}

	init();
}


X509Certificate& X509Certificate::operator = (const X509Certificate& cert)
{
	X509Certificate tmp(cert);
	swap(tmp);
	return *this;
}


void X509Certificate::swap(X509Certificate& cert)
{
	using std::swap;
	swap(cert._issuerName, _issuerName);
	swap(cert._subjectName, _subjectName);
	swap(cert._pCert, _pCert);
}


X509Certificate::~X509Certificate()
{
	CertFreeCertificateContext(_pCert);
}


void X509Certificate::init()
{
	wchar_t data[256];
	CertGetNameStringW(_pCert, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, data, 256);
	Poco::UnicodeConverter::convert(data, _issuerName);
	CertGetNameStringW(_pCert, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, data, 256);
	Poco::UnicodeConverter::convert(data, _subjectName);
}


std::string X509Certificate::commonName() const
{
	return subjectName(NID_COMMON_NAME);
}


std::string X509Certificate::issuerName(NID nid) const
{
	std::string result;
	wchar_t data[256];
	CertGetNameStringW(_pCert, CERT_NAME_ATTR_TYPE, CERT_NAME_ISSUER_FLAG, nid2oid(nid), data, 256);
	Poco::UnicodeConverter::convert(data, result);
	return result;
}


std::string X509Certificate::subjectName(NID nid) const
{
	std::string result;
	wchar_t data[256];
	CertGetNameStringW(_pCert, CERT_NAME_ATTR_TYPE, 0, nid2oid(nid), data, 256);
	Poco::UnicodeConverter::convert(data, result);
	return result;
}


void X509Certificate::extractNames(std::string& cmnName, std::set<std::string>& domainNames) const
{
	domainNames.clear(); 
	// TODO: extract subject alternative names 
	cmnName = commonName();
	if (!cmnName.empty() && domainNames.empty())
	{
		domainNames.insert(cmnName);
	}
}


Poco::DateTime X509Certificate::validFrom() const
{
	Poco::Timestamp ts = Poco::Timestamp::fromFileTimeNP(_pCert->pCertInfo->NotBefore.dwLowDateTime, _pCert->pCertInfo->NotBefore.dwHighDateTime);
	return Poco::DateTime(ts);
}

	
Poco::DateTime X509Certificate::expiresOn() const
{
	Poco::Timestamp ts = Poco::Timestamp::fromFileTimeNP(_pCert->pCertInfo->NotAfter.dwLowDateTime, _pCert->pCertInfo->NotAfter.dwHighDateTime);
	return Poco::DateTime(ts);
}


bool X509Certificate::issuedBy(const X509Certificate& issuerCertificate) const
{
	HCERTSTORE hCertStore = CertOpenSystemStoreW(NULL, L"CA");
	if (!hCertStore) throw Poco::SystemException("Cannot open CA store");
	// TODO
	try
	{
		PCCERT_CONTEXT pIssuer = 0;
		do
		{
			DWORD flags = CERT_STORE_REVOCATION_FLAG | CERT_STORE_SIGNATURE_FLAG | CERT_STORE_TIME_VALIDITY_FLAG;
			pIssuer = CertGetIssuerCertificateFromStore(hCertStore, _pCert, 0, &flags);
			if (pIssuer)
			{
				X509Certificate issuer(pIssuer);
				if (flags & CERT_STORE_NO_CRL_FLAG)
					flags &= ~(CERT_STORE_NO_CRL_FLAG | CERT_STORE_REVOCATION_FLAG);
				if (flags) 
					break;
			}
			else break;
		} 
		while (pIssuer);
	}
	catch (...)
	{
	}
	return false;
}


void* X509Certificate::nid2oid(NID nid)
{
	const char* result = 0;
	switch (nid)
	{
	case NID_COMMON_NAME:
		result = szOID_COMMON_NAME;
		break;
	case NID_COUNTRY:
		result = szOID_COUNTRY_NAME;
		break;
	case NID_LOCALITY_NAME:
		result = szOID_LOCALITY_NAME;
		break;
	case NID_STATE_OR_PROVINCE:
		result = szOID_STATE_OR_PROVINCE_NAME;
		break;
	case NID_ORGANIZATION_NAME:
		result = szOID_ORGANIZATION_NAME;
		break;
	case NID_ORGANIZATION_UNIT_NAME:
		result = szOID_ORGANIZATIONAL_UNIT_NAME;
		break;
	default:
		poco_bugcheck();
		result = "";
		break;
	}
	return const_cast<char*>(result);
}


void X509Certificate::loadCertificate(const std::string& certName, const std::string& certStoreName, bool useMachineStore)
{
	std::wstring wcertStore;
	Poco::UnicodeConverter::convert(certStoreName, wcertStore);
	HCERTSTORE hCertStore;
	if (useMachineStore)
		hCertStore = CertOpenStore(CERT_STORE_PROV_SYSTEM, 0, 0, CERT_SYSTEM_STORE_LOCAL_MACHINE, certStoreName.c_str());
	else
		hCertStore = CertOpenSystemStoreW(0, wcertStore.c_str());

	if (!hCertStore) throw CertificateException("Failed to open certificate store", certStoreName, GetLastError());

	CERT_RDN_ATTR cert_rdn_attr;
	cert_rdn_attr.pszObjId = szOID_COMMON_NAME;
	cert_rdn_attr.dwValueType = CERT_RDN_ANY_TYPE;
	cert_rdn_attr.Value.cbData = (DWORD) certName.size();
	cert_rdn_attr.Value.pbData = (BYTE *) certName.c_str();

	CERT_RDN cert_rdn;
	cert_rdn.cRDNAttr = 1;
	cert_rdn.rgRDNAttr = &cert_rdn_attr;

	_pCert = CertFindCertificateInStore(hCertStore, X509_ASN_ENCODING, 0, CERT_FIND_SUBJECT_ATTR, &cert_rdn, NULL);
	if (!_pCert) 
	{
		CertCloseStore(hCertStore, 0);
		throw NoCertificateException(Poco::format("Failed to find certificate %s in store %s", certName, certStoreName));
	}
	CertCloseStore(hCertStore, 0);
}


void X509Certificate::importCertificate(const std::string& certPath)
{
	Poco::File certFile(certPath);
	if (!certFile.exists()) throw Poco::FileNotFoundException(certPath);
	Poco::File::FileSize size = certFile.getSize();
	if (size > 4096) throw Poco::DataFormatException("certificate file too large", certPath);
	if (size < 32) throw Poco::DataFormatException("certificate file too small", certPath);
	Poco::Buffer<char> buffer(static_cast<std::size_t>(size));
	Poco::FileInputStream istr(certPath);
	istr.read(buffer.begin(), buffer.size());
	if (istr.gcount() != size) throw Poco::IOException("error reading certificate file");
	importCertificate(buffer.begin(), buffer.size());
}


void X509Certificate::importCertificate(const char* pBuffer, std::size_t size)
{
	if (std::memcmp(pBuffer, "-----BEGIN CERTIFICATE-----", 27) == 0)
		importPEMCertificate(pBuffer + 27, size - 27);
	else
		importDERCertificate(pBuffer, size);
}


void X509Certificate::importPEMCertificate(const char* pBuffer, std::size_t size)
{
	Poco::Buffer<char> derBuffer(size);
	std::size_t derSize = 0;

	const char* pemBegin = pBuffer;
	const char* pemEnd = pemBegin + (size - 25);
	while (pemEnd > pemBegin && std::memcmp(pemEnd, "-----END CERTIFICATE-----", 25) != 0) --pemEnd;
	if (pemEnd == pemBegin) throw Poco::DataFormatException("Not a valid PEM file - end marker missing");

	Poco::MemoryInputStream istr(pemBegin, pemEnd - pemBegin);
	Poco::Base64Decoder dec(istr);

	char* derBegin = derBuffer.begin();
	char* derEnd = derBegin;

	int ch = dec.get();
	while (ch != -1) 
	{
		*derEnd++ = static_cast<char>(ch);
		ch = dec.get();
	}

	importDERCertificate(derBegin, derEnd - derBegin);
}


void X509Certificate::importDERCertificate(const char* pBuffer, std::size_t size)
{
	_pCert = CertCreateCertificateContext(X509_ASN_ENCODING, reinterpret_cast<const BYTE*>(pBuffer), static_cast<DWORD>(size));
	if (!_pCert)
	{
		throw Poco::DataFormatException("Failed to load certificate from file", GetLastError());
	}
}


} } // namespace Poco::Net
