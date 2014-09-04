//
// X509Certificate.cpp
//
// $Id: //poco/1.4/Crypto/src/X509Certificate.cpp#1 $
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
#include "Poco/StreamCopier.h"
#include "Poco/String.h"
#include "Poco/DateTimeParser.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include "Poco/FileStream.h"
#include "Poco/UnicodeConverter.h"
#include <sstream>


namespace Poco {
namespace Net {


X509Certificate::X509Certificate(std::istream& istr):
	_pCert(0)
{	
	load(istr);
}


X509Certificate::X509Certificate(const std::string& path):
	_pCert(0)
{
	load(path);
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


void X509Certificate::load(std::istream& istr)
{
	poco_assert (!_pCert);
		
	// TODO

	init();
}


void X509Certificate::load(const std::string& path)
{
	Poco::FileInputStream istr(path);
	load(istr);
}


void X509Certificate::save(std::ostream& stream) const
{
	// TODO
}


void X509Certificate::save(const std::string& path) const
{
	Poco::FileOutputStream ostr(path);
	save(ostr);
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


} } // namespace Poco::Net
