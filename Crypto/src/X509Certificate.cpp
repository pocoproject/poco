//
// X509Certificate.cpp
//
// Library: Crypto
// Package: Certificate
// Module:  X509Certificate
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/X509Certificate.h"
#include "Poco/Crypto/CryptoException.h"
#include "Poco/StreamCopier.h"
#include "Poco/String.h"
#include "Poco/DateTimeParser.h"
#include "Poco/Format.h"
#include <sstream>
#include <openssl/pem.h>
#ifdef _WIN32
// fix for WIN32 header conflict
#undef X509_NAME
#endif
#include <openssl/x509v3.h>
#include <openssl/err.h>
#include <openssl/evp.h>


namespace Poco {
namespace Crypto {


X509Certificate::X509Certificate():
	_pCert(X509_new())
{
	X509_set_version(_pCert, 2);
}

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


X509Certificate::X509Certificate(X509* pCert):
	_pCert(pCert)
{
	poco_check_ptr(_pCert);

	init();
}


X509Certificate::X509Certificate(X509* pCert, bool shared):
	_pCert(pCert)
{
	poco_check_ptr(_pCert);
	
	if (shared)
	{
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
		X509_up_ref(_pCert);
#else
		_pCert->references++;
#endif
	}

	init();
}


X509Certificate::X509Certificate(const X509Certificate& cert):
	_issuerName(cert._issuerName),
	_subjectName(cert._subjectName),
	_serialNumber(cert._serialNumber),
	_pCert(0)
{
	duplicate(cert._pCert, &_pCert);
}

X509Certificate::X509Certificate(X509Certificate&& other) :
	_issuerName(),
	_subjectName(),
	_serialNumber(),
	_pCert(0)
{
	swap(*this, other);
}


X509Certificate& X509Certificate::operator=(X509Certificate other)
{
	swap(*this, other);
	return *this;
}


void X509Certificate::swap(X509Certificate &first, X509Certificate &second)
{
	using std::swap;
	swap(first._issuerName, second._issuerName);
	swap(first._subjectName, second._subjectName);
	swap(first._serialNumber, second._serialNumber);
	swap(first._pCert, second._pCert);
}

void X509Certificate::swap(X509Certificate& cert)
{
	swap(cert, *this);
}


X509Certificate::~X509Certificate()
{
	X509_free(_pCert);
}


void X509Certificate::load(std::istream& istr)
{
	poco_assert (!_pCert);

	std::stringstream certStream;
	Poco::StreamCopier::copyStream(istr, certStream);
	std::string cert = certStream.str();

	BIO *pBIO = BIO_new_mem_buf(const_cast<char*>(cert.data()), static_cast<int>(cert.size()));
	if (!pBIO) throw Poco::IOException("Cannot create BIO for reading certificate");
	_pCert = PEM_read_bio_X509(pBIO, 0, 0, 0);
	BIO_free(pBIO);

	if (!_pCert) throw Poco::IOException("Failed to load certificate from stream");

	init();
}


void X509Certificate::load(const std::string& path)
{
	poco_assert (!_pCert);

	BIO *pBIO = BIO_new(BIO_s_file());
	if (!pBIO) throw Poco::IOException("Cannot create BIO for reading certificate file", path);
	if (!BIO_read_filename(pBIO, path.c_str()))
	{
		BIO_free(pBIO);
		throw Poco::OpenFileException("Cannot open certificate file for reading", path);
	}

	_pCert = PEM_read_bio_X509(pBIO, 0, 0, 0);
	BIO_free(pBIO);

	if (!_pCert) throw Poco::ReadFileException("Faild to load certificate from", path);

	init();
}


void X509Certificate::save(std::ostream& stream) const
{
	BIO *pBIO = BIO_new(BIO_s_mem());
	if (!pBIO) throw Poco::IOException("Cannot create BIO for writing certificate");
	try
	{
		if (!PEM_write_bio_X509(pBIO, _pCert))
			throw Poco::IOException("Failed to write certificate to stream");

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


void X509Certificate::save(const std::string& path) const
{
	BIO *pBIO = BIO_new(BIO_s_file());
	if (!pBIO) throw Poco::IOException("Cannot create BIO for reading certificate file", path);
	if (!BIO_write_filename(pBIO, const_cast<char*>(path.c_str())))
	{
		BIO_free(pBIO);
		throw Poco::CreateFileException("Cannot create certificate file", path);
	}
	try
	{
		if (!PEM_write_bio_X509(pBIO, _pCert))
			throw Poco::WriteFileException("Failed to write certificate to file", path);
	}
	catch (...)
	{
		BIO_free(pBIO);
		throw;
	}
	BIO_free(pBIO);
}


void X509Certificate::init()
{
	char buffer[NAME_BUFFER_SIZE];
	X509_NAME_oneline(X509_get_issuer_name(_pCert), buffer, sizeof(buffer));
	_issuerName = buffer;
	X509_NAME_oneline(X509_get_subject_name(_pCert), buffer, sizeof(buffer));
	_subjectName = buffer;
	BIGNUM* pBN = ASN1_INTEGER_to_BN(X509_get_serialNumber(const_cast<X509*>(_pCert)), 0);
	if (pBN)
	{
		char* pSN = BN_bn2hex(pBN);
		if (pSN)
		{
			_serialNumber = pSN;
			OPENSSL_free(pSN);
		}
		BN_free(pBN);
	}
}


void X509Certificate::setSerialNumber(long serial)
{
	ASN1_INTEGER* asn1Serial = ASN1_INTEGER_new();
	ASN1_INTEGER_set(asn1Serial, serial);
	X509_set_serialNumber(_pCert, asn1Serial);
	ASN1_INTEGER_free(asn1Serial);

	init();
}


void X509Certificate::addIssuer(X509Certificate::NID nid, const std::string& name)
{
	const void* data = name.c_str();
	unsigned char* bytes = static_cast<unsigned char*>(const_cast<void*>(data));
	X509_NAME* x509name = X509_get_issuer_name(_pCert);
	X509_NAME_add_entry_by_NID(x509name, nid, MBSTRING_ASC, bytes, -1, -1, 0);
	X509_set_issuer_name(_pCert, x509name);

	init();
}


std::string X509Certificate::commonName() const
{
	return subjectName(NID_COMMON_NAME);
}


void X509Certificate::setCommonName(const std::string& cn)
{
	addSubject(NID_COMMON_NAME, cn);
}


std::string X509Certificate::issuerName(NID nid) const
{
	if (X509_NAME* issuer = X509_get_issuer_name(_pCert))
	{
		char buffer[NAME_BUFFER_SIZE];
		if (X509_NAME_get_text_by_NID(issuer, nid, buffer, sizeof(buffer)) >= 0)
			return std::string(buffer);
	}
	return std::string();
}


void X509Certificate::addSubject(NID nid, const std::string& name)
{
	const void* data = name.c_str();
	unsigned char* bytes = static_cast<unsigned char*>(const_cast<void*>(data));
	X509_NAME* x509name = X509_get_subject_name(_pCert);
	X509_NAME_add_entry_by_NID(x509name, nid, MBSTRING_ASC, bytes, -1, -1, 0);
	X509_set_subject_name(_pCert, x509name);

	init();
}


std::string X509Certificate::subjectName(NID nid) const
{
	X509_NAME* subj = X509_get_subject_name(_pCert);
	if (subj)
	{
		char buffer[NAME_BUFFER_SIZE];
		if (X509_NAME_get_text_by_NID(subj, nid, buffer, sizeof(buffer)) >= 0)
			return std::string(buffer);
	}
	return std::string();
}


void X509Certificate::extractNames(std::string& cmnName, std::set<std::string>& domainNames) const
{
	domainNames.clear();
	if (STACK_OF(GENERAL_NAME)* names = static_cast<STACK_OF(GENERAL_NAME)*>(X509_get_ext_d2i(_pCert, NID_subject_alt_name, 0, 0)))
	{
		for (int i = 0; i < sk_GENERAL_NAME_num(names); ++i)
		{
			const GENERAL_NAME* name = sk_GENERAL_NAME_value(names, i);
			if (name->type == GEN_DNS)
			{
				const char* data = reinterpret_cast<char*>(ASN1_STRING_data(name->d.ia5));
				std::size_t len = ASN1_STRING_length(name->d.ia5);
				domainNames.insert(std::string(data, len));
			}
		}
		GENERAL_NAMES_free(names);
	}

	cmnName = commonName();
	if (!cmnName.empty() && domainNames.empty())
	{
		domainNames.insert(cmnName);
	}
}


void X509Certificate::setValidFrom(DateTime from)
{
	Timestamp timestamp = from.timestamp();
	time_t time = timestamp.epochTime();
	ASN1_TIME* asn1time = ASN1_TIME_new();
	ASN1_TIME_set(asn1time, time);
	X509_set_notBefore(_pCert, asn1time);
	ASN1_TIME_free(asn1time);
}


Poco::DateTime X509Certificate::validFrom() const
{
	ASN1_TIME* certTime = X509_get_notBefore(_pCert);
	std::string dateTime(reinterpret_cast<char*>(certTime->data));
	std::string fmt = "%y%m%d%H%M%S";
	if(certTime->type == V_ASN1_GENERALIZEDTIME)
	{
		fmt = "%Y%m%d%H%M%S";
	}
	int tzd;
	return DateTimeParser::parse(fmt, dateTime, tzd);
}
    

void X509Certificate::setExpiresOn(DateTime expires)
{
	Timestamp timestamp = expires.timestamp();
	time_t time = timestamp.epochTime();
	ASN1_TIME* asn1time = ASN1_TIME_new();
	ASN1_TIME_set(asn1time, time);
	X509_set_notAfter(_pCert, asn1time);
	ASN1_TIME_free(asn1time);
}

	
Poco::DateTime X509Certificate::expiresOn() const
{
	ASN1_TIME* certTime = X509_get_notAfter(_pCert);
	std::string dateTime(reinterpret_cast<char*>(certTime->data));
	std::string fmt = "%y%m%d%H%M%S";
	if(certTime->type == V_ASN1_GENERALIZEDTIME)
	{
		fmt = "%Y%m%d%H%M%S";
	}
	int tzd;
	return DateTimeParser::parse(fmt, dateTime, tzd);
}


void X509Certificate::setPublicKey(const EVPPKey &pkey)
{
	X509_set_pubkey(_pCert, const_cast<EVP_PKEY*>(static_cast<const EVP_PKEY*>(pkey)));
}


EVPPKey X509Certificate::publicKey() const
{
	EVP_PKEY* k = X509_get_pubkey(_pCert);
	return EVPPKey(k);
}


void X509Certificate::addExtension(const X509Extension &x509Extension)
{
	X509_EXTENSION* ext = const_cast<X509_EXTENSION*>(static_cast<const X509_EXTENSION*>(x509Extension));
	X509_add_ext(_pCert, ext, -1);
}


X509Extension::List X509Certificate::findExtensionByNID(int nid)
{
	X509Extension::List extensions;
	X509_EXTENSION *new_ex = 0;
	int index = -1;
	do {
		index = X509_get_ext_by_NID(_pCert, nid, index);
		if(index == -1)
			break;

		X509_EXTENSION* ext = X509_get_ext(_pCert, index);
		new_ex = X509_EXTENSION_dup(ext);
		extensions.push_back(X509Extension(new_ex));
	}
	while(index >= 0);

	return extensions;
}


bool X509Certificate::issuedBy(const X509Certificate& issuerCertificate) const
{
	X509* pCert = const_cast<X509*>(_pCert);
	X509* pIssuerCert = const_cast<X509*>(static_cast<const X509*>(issuerCertificate));
	EVP_PKEY* pIssuerPublicKey = X509_get_pubkey(pIssuerCert);
	if (!pIssuerPublicKey) throw Poco::InvalidArgumentException("Issuer certificate has no public key");
	int rc = X509_verify(pCert, pIssuerPublicKey);
	EVP_PKEY_free(pIssuerPublicKey);
	return rc == 1;
}


bool X509Certificate::sign(const EVPPKey &pkey, const std::string& mdstr)
{
	const EVP_MD* md = EVP_get_digestbyname(mdstr.c_str());
	int rc = X509_sign(_pCert, const_cast<EVP_PKEY*>(static_cast<const EVP_PKEY*>(pkey)), md);
	return rc > 0;
}


bool X509Certificate::equals(const X509Certificate& otherCertificate) const
{
	X509* pCert = const_cast<X509*>(_pCert);
	X509* pOtherCert = const_cast<X509*>(static_cast<const X509*>(otherCertificate));
	return X509_cmp(pCert, pOtherCert) == 0;
}


std::string X509Certificate::signatureAlgorithm() const
{
	int sigNID = NID_undef;

#if (OPENSSL_VERSION_NUMBER >=  0x1010000fL) && !defined(LIBRESSL_VERSION_NUMBER)
	sigNID = X509_get_signature_nid(_pCert);
#else
	poco_check_ptr(_pCert->sig_alg);
	sigNID = OBJ_obj2nid(_pCert->sig_alg->algorithm);
#endif

	if (sigNID != NID_undef)
	{
		const char* pAlgName = OBJ_nid2ln(sigNID);
		if (pAlgName) return std::string(pAlgName);
		else throw OpenSSLException(Poco::format("X509Certificate::"
			"signatureAlgorithm(): OBJ_nid2ln(%d)", sigNID));
	}
	else
		throw NotFoundException("X509Certificate::signatureAlgorithm()");

	return "";
}


X509Certificate::List X509Certificate::readPEM(const std::string& pemFileName)
{
	List caCertList;
	BIO* pBIO = BIO_new_file(pemFileName.c_str(), "r");
	if (pBIO == NULL) throw OpenFileException("X509Certificate::readPEM()");
	X509* x = PEM_read_bio_X509(pBIO, NULL, 0, NULL);
	if (!x) throw OpenSSLException(Poco::format("X509Certificate::readPEM(%s)", pemFileName));
	while(x)
	{
		caCertList.push_back(X509Certificate(x));
		x = PEM_read_bio_X509(pBIO, NULL, 0, NULL);
	}
	BIO_free(pBIO);
	return caCertList;
}


void X509Certificate::writePEM(const std::string& pemFileName, const List& list)
{
	BIO* pBIO = BIO_new_file(pemFileName.c_str(), "a");
	if (pBIO == NULL) throw OpenFileException("X509Certificate::writePEM()");
	List::const_iterator it = list.begin();
	List::const_iterator end = list.end();
	for (; it != end; ++it)
	{
		if (!PEM_write_bio_X509(pBIO, const_cast<X509*>(static_cast<const X509*>(*it))))
		{
			throw OpenSSLException("X509Certificate::writePEM()");
		}
	}
	BIO_free(pBIO);
}


X509* X509Certificate::duplicate(const X509* pFromExtension, X509** pToExtension)
{
	*pToExtension = X509_dup(const_cast<X509*>(pFromExtension));
	return *pToExtension;
}


void X509Certificate::print(std::ostream& out) const
{
	out << "subjectName: " << subjectName() << std::endl;
	out << "issuerName: " << issuerName() << std::endl;
	out << "commonName: " << commonName() << std::endl;
	out << "country: " << subjectName(X509Certificate::NID_COUNTRY) << std::endl;
	out << "localityName: " << subjectName(X509Certificate::NID_LOCALITY_NAME) << std::endl;
	out << "stateOrProvince: " << subjectName(X509Certificate::NID_STATE_OR_PROVINCE) << std::endl;
	out << "organizationName: " << subjectName(X509Certificate::NID_ORGANIZATION_NAME) << std::endl;
	out << "organizationUnitName: " << subjectName(X509Certificate::NID_ORGANIZATION_UNIT_NAME) << std::endl;
	out << "emailAddress: " << subjectName(X509Certificate::NID_PKCS9_EMAIL_ADDRESS) << std::endl;
	out << "serialNumber: " << subjectName(X509Certificate::NID_SERIAL_NUMBER) << std::endl;
}


void X509Certificate::printAll(std::ostream& out) const
{
	X509_NAME *subj = X509_get_subject_name(_pCert);

	for (int i = 0; i < X509_NAME_entry_count(subj); ++i)
	{
		X509_NAME_ENTRY* e = X509_NAME_get_entry(subj, i);
		ASN1_STRING* d = X509_NAME_ENTRY_get_data(e);
		unsigned char* str = ASN1_STRING_data(d);
		out << (char*) str << std::endl;
	}
}


} } // namespace Poco::Crypto
