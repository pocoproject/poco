//
// X509RevocationList.cpp
//
//
// Library: Crypto
// Package: Certificate
// Module:  X509RevocationList
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/X509RevocationList.h"
#include "Poco/StreamCopier.h"
#include "Poco/String.h"
#include "Poco/DateTimeParser.h"
#include <sstream>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/err.h>
#include <openssl/evp.h>


namespace Poco {
namespace Crypto {


X509RevocationList::X509RevocationList():
	_issuerName(),
	_pCrl(X509_CRL_new())
{
	X509_CRL_set_version(_pCrl, 2);
}

X509RevocationList::X509RevocationList(std::istream& istr):
	_issuerName(),
	_pCrl(nullptr)
{	
	load(istr);
}


X509RevocationList::X509RevocationList(const std::string& path):
	_issuerName(),
	_pCrl(nullptr)
{
	load(path);
}


X509RevocationList::X509RevocationList(X509_CRL* pCert):
	_issuerName(),
	_pCrl(pCert)
{
	poco_check_ptr(_pCrl);

	init();
}


X509RevocationList::X509RevocationList(X509_CRL* pCert, bool shared):
	_issuerName(),
	_pCrl(pCert)
{
	poco_check_ptr(_pCrl);
	
	if (shared)
	{
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
		X509_CRL_up_ref(_pCrl);
#else
		_pCrl->references++;
#endif
	}

	init();
}


X509RevocationList::X509RevocationList(const X509RevocationList& cert):
	_issuerName(cert._issuerName),
	_pCrl(nullptr)
{
	duplicate(cert._pCrl, &_pCrl);
}


X509RevocationList::X509RevocationList(X509RevocationList&& other):
	_issuerName(),
	_pCrl(nullptr)
{
	swap(*this, other);
}


X509RevocationList& X509RevocationList::operator = (X509RevocationList other)
{
	swap(*this, other);
	return *this;
}


void X509RevocationList::swap(X509RevocationList& first, X509RevocationList& second)
{
	using std::swap;
	swap(first._issuerName, second._issuerName);
	swap(first._pCrl, second._pCrl);
}


X509RevocationList::~X509RevocationList()
{
	X509_CRL_free(_pCrl);
}


void X509RevocationList::load(std::istream& istr)
{
	poco_assert (!_pCrl);
		
	std::stringstream certStream;
	Poco::StreamCopier::copyStream(istr, certStream);
	std::string cert = certStream.str();
		
	BIO *pBIO = BIO_new_mem_buf(const_cast<char*>(cert.data()), static_cast<int>(cert.size()));
	if (!pBIO) throw Poco::IOException("Cannot create BIO for reading crl");
	_pCrl = PEM_read_bio_X509_CRL(pBIO, nullptr, nullptr, nullptr);
	BIO_free(pBIO);
	
	if (!_pCrl) throw Poco::IOException("Faild to load crl from stream");

	init();
}


void X509RevocationList::load(const std::string& path)
{
	poco_assert (!_pCrl);

	BIO *pBIO = BIO_new(BIO_s_file());
	if (!pBIO) throw Poco::IOException("Cannot create BIO for reading crl file", path);
	if (!BIO_read_filename(pBIO, const_cast<char*>(path.c_str())))
	{
		BIO_free(pBIO);
		throw Poco::OpenFileException("Cannot open crl file for reading", path);
	}
	
	_pCrl = PEM_read_bio_X509_CRL(pBIO, nullptr, nullptr, nullptr);
	BIO_free(pBIO);
	
	if (!_pCrl) throw Poco::ReadFileException("Faild to load crl from", path);

	init();
}


void X509RevocationList::save(std::ostream& stream) const
{
	BIO *pBIO = BIO_new(BIO_s_mem());
	if (!pBIO) throw Poco::IOException("Cannot create BIO for writing crl");
	try
	{
		if (!PEM_write_bio_X509_CRL(pBIO, _pCrl))
			throw Poco::IOException("Failed to write crl to stream");

		char *pData;
		long size;
		size = BIO_get_mem_data(pBIO, &pData);
		stream.write(pData, size);
	}
	catch (...)
	{
		BIO_free(pBIO);
		throw;
	}
	BIO_free(pBIO);
}


void X509RevocationList::save(const std::string& path) const
{
	BIO *pBIO = BIO_new(BIO_s_file());
	if (!pBIO) throw Poco::IOException("Cannot create BIO for reading crl file", path);
	if (!BIO_write_filename(pBIO, const_cast<char*>(path.c_str())))
	{
		BIO_free(pBIO);
		throw Poco::CreateFileException("Cannot create crl file", path);
	}
	try
	{
		if (!PEM_write_bio_X509_CRL(pBIO, _pCrl))
			throw Poco::WriteFileException("Failed to write crl to file", path);
	}
	catch (...)
	{
		BIO_free(pBIO);
		throw;
	}
	BIO_free(pBIO);
}


void X509RevocationList::init()
{
	char buffer[NAME_BUFFER_SIZE];
	X509_NAME_oneline(X509_CRL_get_issuer(_pCrl), buffer, sizeof(buffer));
	_issuerName = buffer;
}


long X509RevocationList::version() const
{
	return X509_CRL_get_version(_pCrl) + 1;
}


void X509RevocationList::addIssuer(X509RevocationList::NID nid, const std::string& name)
{
	const void* data = name.c_str();
	unsigned char* bytes = static_cast<unsigned char*>(const_cast<void*>(data));
	X509_NAME* x509name = X509_CRL_get_issuer(_pCrl);
	X509_NAME_add_entry_by_NID(x509name, nid, MBSTRING_ASC, bytes, -1, -1, 0);
	X509_CRL_set_issuer_name(_pCrl, x509name);

	init();
}


std::string X509RevocationList::issuerName(NID nid) const
{
	if (X509_NAME* issuer = X509_CRL_get_issuer(_pCrl))
    {
		char buffer[NAME_BUFFER_SIZE];
		if (X509_NAME_get_text_by_NID(issuer, nid, buffer, sizeof(buffer)) >= 0)
			return std::string(buffer);
    }
    return std::string();
}


void X509RevocationList::extractNames(std::set<std::string>& domainNames) const
{
	domainNames.clear(); 
	if (STACK_OF(GENERAL_NAME)* names = static_cast<STACK_OF(GENERAL_NAME)*>(X509_CRL_get_ext_d2i(_pCrl, NID_subject_alt_name, nullptr, nullptr)))
    {
		for (int i = 0; i < sk_GENERAL_NAME_num(names); ++i)
        {
			const GENERAL_NAME* name = sk_GENERAL_NAME_value(names, i);
			if (name->type == GEN_DNS)
			{
				const char* data = reinterpret_cast<char*>(ASN1_STRING_data(name->d.ia5));
				std::size_t len = static_cast<std::size_t>(ASN1_STRING_length(name->d.ia5));
				domainNames.insert(std::string(data, len));
            }
		}
		GENERAL_NAMES_free(names);
	}
}


void X509RevocationList::setLastUpdate(DateTime dateTime)
{
	Timestamp timestamp = dateTime.timestamp();
	time_t time = timestamp.epochTime();
	ASN1_TIME* asn1time = ASN1_TIME_new();
	ASN1_TIME_set(asn1time, time);
	X509_CRL_set_lastUpdate(_pCrl, asn1time);
	ASN1_TIME_free(asn1time);
}


Poco::DateTime X509RevocationList::lastUpdate() const
{
	ASN1_TIME* certTime = X509_CRL_get_lastUpdate(_pCrl);
	std::string dateTime(reinterpret_cast<char*>(certTime->data));
	std::string fmt = "%y%m%d%H%M%S";
	if(certTime->type == V_ASN1_GENERALIZEDTIME)
	{
		fmt = "%Y%m%d%H%M%S";
	}
	int tzd;
	return DateTimeParser::parse(fmt, dateTime, tzd);
}
    

void X509RevocationList::setNextUpdate(DateTime expires)
{
	Timestamp timestamp = expires.timestamp();
	time_t time = timestamp.epochTime();
	ASN1_TIME* asn1time = ASN1_TIME_new();
	ASN1_TIME_set(asn1time, time);
	X509_CRL_set_nextUpdate(_pCrl, asn1time);
	ASN1_TIME_free(asn1time);
}

	
Poco::DateTime X509RevocationList::nextUpdate() const
{
	ASN1_TIME* certTime = X509_CRL_get_nextUpdate(_pCrl);
	std::string dateTime(reinterpret_cast<char*>(certTime->data));
	std::string fmt = "%y%m%d%H%M%S";
	if(certTime->type == V_ASN1_GENERALIZEDTIME)
	{
		fmt = "%Y%m%d%H%M%S";
	}
	int tzd;
	return DateTimeParser::parse(fmt, dateTime, tzd);
}


void X509RevocationList::addExtension(const X509Extension &x509Extension)
{
	X509_EXTENSION* ext = const_cast<X509_EXTENSION*>(static_cast<const X509_EXTENSION*>(x509Extension));
	X509_CRL_add_ext(_pCrl, ext, -1);
}


void X509RevocationList::addRevoked(const X509Revoked &revoked)
{
	X509_REVOKED* rev = const_cast<X509_REVOKED*>(static_cast<const X509_REVOKED*>(revoked));
	X509_REVOKED* newRev;
	X509Revoked::duplicate(rev, &newRev);
	X509_CRL_add0_revoked(_pCrl, newRev);
}


X509Revoked X509RevocationList::findRevokedBySerialNumber(long serialNumber)
{
	ASN1_INTEGER* asn1Serial = ASN1_INTEGER_new();
	ASN1_INTEGER_set(asn1Serial, serialNumber);
	X509_REVOKED *ret;
	int rc = X509_CRL_get0_by_serial(_pCrl, &ret, asn1Serial);
	X509Revoked rev;
	if(rc)
	{
		rev = X509Revoked(ret);
	}
	return rev;
}


X509Revoked X509RevocationList::findRevokedByCertificate(const X509Certificate &certificate)
{
	X509* x509 = const_cast<X509*>(static_cast<const X509*>(certificate));
	X509_REVOKED *ret;
	int rc = X509_CRL_get0_by_cert(_pCrl, &ret, x509);
	X509Revoked rev;
	if(rc)
	{
		rev = X509Revoked(ret);
	}
	return rev;
}


X509Revoked::List X509RevocationList::getAllRevoked() const
{
	X509Revoked::List revokedList;
	STACK_OF(X509_REVOKED)* revs = X509_CRL_get_REVOKED(_pCrl);
	for (int i = 0; i < sk_X509_REVOKED_num(revs); i++)
	{
		X509_REVOKED *rv = sk_X509_REVOKED_value(revs, i);
		revokedList.push_back(X509Revoked(rv));
	}
	return revokedList;
}


X509Extension::List X509RevocationList::findExtensionByNID(int nid)
{
	X509Extension::List extensions;
	int index = -1;
	do {
		index = X509_CRL_get_ext_by_NID(_pCrl, nid, index);
		X509_EXTENSION* ext = X509_CRL_get_ext(_pCrl, index);
		extensions.push_back(X509Extension(ext));
	}
	while(index >= 0);

	return extensions;
}


bool X509RevocationList::verify(const EVPPKey &key) const
{
	EVP_PKEY* pIssuerPublicKey = const_cast<EVP_PKEY*>(static_cast<const EVP_PKEY*>(key));
	int rc = X509_CRL_verify(_pCrl, pIssuerPublicKey);
	return rc == 1;
}


bool X509RevocationList::sign(const EVPPKey &pkey, const std::string& mdstr)
{
	const char* mdc = mdstr.c_str();
	if(std::string("default").compare(mdstr))
	{
		int defNid = 0;
		EVP_PKEY_get_default_digest_nid(const_cast<EVP_PKEY*>(static_cast<const EVP_PKEY*>(pkey)), &defNid);
		mdc = OBJ_nid2sn(defNid);
	}
	const EVP_MD* md = EVP_get_digestbyname(mdc);
	X509_CRL_sign(_pCrl, const_cast<EVP_PKEY*>(static_cast<const EVP_PKEY*>(pkey)), md);
	return true;
}


bool X509RevocationList::equals(const X509RevocationList& otherCertificate) const
{
	const X509_CRL* pOtherCert = otherCertificate;
	return X509_CRL_cmp(_pCrl, pOtherCert) == 0;
}

X509_CRL* X509RevocationList::duplicate(const X509_CRL *pFromCRL, X509_CRL **pToCRL)
{
	*pToCRL = X509_CRL_dup(const_cast<X509_CRL*>(pFromCRL));
	return *pToCRL;
}


} } // namespace Poco::Crypto
