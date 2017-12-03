//
// X509Request.cpp
//
// $Id: $
//
// Library: Crypto
// Package: Certificate
// Module:  X509Request
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/X509Request.h"
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


X509Request::X509Request():
	_pRequest(X509_REQ_new())
{
	X509_REQ_set_version(_pRequest, 2);
}

X509Request::X509Request(std::istream& istr):
	_pRequest(nullptr)
{	
	load(istr);
}


X509Request::X509Request(const std::string& path):
	_pRequest(nullptr)
{
	load(path);
}


X509Request::X509Request(X509_REQ* pReq):
	_pRequest(pReq)
{
	poco_check_ptr(_pRequest);

	init();
}


#if OPENSSL_VERSION_NUMBER <= 0x10100000L
X509Request::X509Request(X509_REQ* pReq, bool shared):
	_pRequest(pReq)
{
	poco_check_ptr(_pRequest);
	
	if (shared)
	{
		_pRequest->references++;
	}

	init();
}
#endif


X509Request::X509Request(const X509Request& other):
	_subjectName(other._subjectName),
	_pRequest(nullptr)
{
	duplicate(other._pRequest, &_pRequest);
}


X509Request::X509Request(X509Request &&other):
	_subjectName(),
	_pRequest(nullptr)
{
	swap(*this, other);
}


X509Request& X509Request::operator = (X509Request other)
{
	swap(*this, other);
	return *this;
}

void X509Request::swap(X509Request& first, X509Request& second)
{
	using std::swap;
	swap(first._subjectName, second._subjectName);
	swap(first._pRequest, second._pRequest);
}


X509Request::~X509Request()
{
	X509_REQ_free(_pRequest);
}


void X509Request::load(std::istream& istr)
{
	poco_assert (!_pRequest);
		
	std::stringstream certStream;
	Poco::StreamCopier::copyStream(istr, certStream);
	std::string cert = certStream.str();
		
	BIO *pBIO = BIO_new_mem_buf(const_cast<char*>(cert.data()), static_cast<int>(cert.size()));
	if (!pBIO) throw Poco::IOException("Cannot create BIO for reading certificate");
	_pRequest = PEM_read_bio_X509_REQ(pBIO, nullptr, nullptr, nullptr);
	BIO_free(pBIO);
	
	if (!_pRequest) throw Poco::IOException("Faild to load certificate from stream");

	init();
}


void X509Request::load(const std::string& path)
{
	poco_assert (!_pRequest);

	BIO *pBIO = BIO_new(BIO_s_file());
	if (!pBIO) throw Poco::IOException("Cannot create BIO for reading certificate file", path);
	if (!BIO_read_filename(pBIO, const_cast<char*>(path.c_str())))
	{
		BIO_free(pBIO);
		throw Poco::OpenFileException("Cannot open certificate file for reading", path);
	}
	
	_pRequest = PEM_read_bio_X509_REQ(pBIO, nullptr, nullptr, nullptr);
	BIO_free(pBIO);
	
	if (!_pRequest) throw Poco::ReadFileException("Faild to load certificate from", path);

	init();
}


void X509Request::save(std::ostream& stream) const
{
	BIO *pBIO = BIO_new(BIO_s_mem());
	if (!pBIO) throw Poco::IOException("Cannot create BIO for writing certificate");
	try
	{
		if (!PEM_write_bio_X509_REQ(pBIO, _pRequest))
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


void X509Request::save(const std::string& path) const
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
		if (!PEM_write_bio_X509_REQ(pBIO, _pRequest))
			throw Poco::WriteFileException("Failed to write certificate to file", path);
	}
	catch (...)
	{
		BIO_free(pBIO);
		throw;
	}
	BIO_free(pBIO);
}


void X509Request::init()
{
	char buffer[NAME_BUFFER_SIZE];
	X509_NAME_oneline(X509_REQ_get_subject_name(_pRequest), buffer, sizeof(buffer));
	_subjectName = buffer;
}


long X509Request::version() const
{
	return X509_REQ_get_version(_pRequest) + 1;
}


std::string X509Request::commonName() const
{
	return subjectName(NID_COMMON_NAME);
}


void X509Request::setCommonName(const std::string& cn)
{
	addSubject(NID_COMMON_NAME, cn);
}


void X509Request::addSubject(NID nid, const std::string& name)
{
	const void* data = name.c_str();
	unsigned char* bytes = static_cast<unsigned char*>(const_cast<void*>(data));
	X509_NAME* x509name = X509_REQ_get_subject_name(_pRequest);
	X509_NAME_add_entry_by_NID(x509name, nid, MBSTRING_ASC, bytes, -1, -1, 0);
	X509_REQ_set_subject_name(_pRequest, x509name);

	init();
}


std::string X509Request::subjectName(NID nid) const
{
	if (X509_NAME* subj = X509_REQ_get_subject_name(_pRequest))
    {
		char buffer[NAME_BUFFER_SIZE];
		if (X509_NAME_get_text_by_NID(subj, nid, buffer, sizeof(buffer)) >= 0)
			return std::string(buffer);
    }
    return std::string();
}


void X509Request::setPublicKey(const EVPPKey &pkey)
{
	X509_REQ_set_pubkey(_pRequest, const_cast<EVP_PKEY*>(static_cast<const EVP_PKEY*>(pkey)));
}


EVPPKey X509Request::publicKey() const
{
	EVP_PKEY* k = X509_REQ_get_pubkey(_pRequest);
	return EVPPKey(k);
}


void X509Request::addExtension(const X509Extension &x509Extension)
{
	X509Extension::List extList;
	extList.push_back(x509Extension);
	setExtensions(extList);
}


void X509Request::setExtensions(const X509Extension::List &x509Extensions)
{
	if(x509Extensions.size() > 0)
	{
		STACK_OF(X509_EXTENSION)* extensions = nullptr;
		for (decltype(x509Extensions.size()) i = 0; i < x509Extensions.size(); ++i)
		{
			X509_EXTENSION* ext = const_cast<X509_EXTENSION*>(static_cast<const X509_EXTENSION*>(x509Extensions[i]));
			X509v3_add_ext(&extensions, ext, -1);
		}

		STACK_OF(X509_EXTENSION)* skOldExt = X509_REQ_get_extensions(_pRequest);
        for (int i = 0;i < sk_X509_EXTENSION_num(skOldExt); ++i)
        {
            X509_EXTENSION* oldExt = sk_X509_EXTENSION_value(skOldExt, i);
            X509v3_add_ext(&extensions, oldExt, -1);
        }

		// X509_REQ_add_extensions() adds at every call new NID_ext_req attribute on the x509. Why? I don't know ....
		// X509_REQ_get_extensions() get only the first NID_ext_req ... so we should delete the NID_ext_req before
		// we add a extension !!
		int idx = X509_REQ_get_attr_by_NID(_pRequest, NID_ext_req, -1);
		if (idx != -1)
			X509_REQ_delete_attr(_pRequest, idx);

		X509_REQ_add_extensions(_pRequest, extensions);
		sk_X509_EXTENSION_pop_free(extensions, X509_EXTENSION_free);
	}
}


X509Extension::List X509Request::extensions()
{
	STACK_OF(X509_EXTENSION)* exts = X509_REQ_get_extensions(_pRequest);
	X509Extension::List extensions;
	for (int i = 0;i < sk_X509_EXTENSION_num(exts); ++i)
	{
		X509_EXTENSION *ext = sk_X509_EXTENSION_value(exts, i);
		extensions.push_back(X509Extension(ext));
	}
	sk_X509_EXTENSION_free(exts);
	return extensions;
}


bool X509Request::issuedBy(const X509Request& issuerCertificate) const
{
	X509_REQ* pCert = const_cast<X509_REQ*>(_pRequest);
	X509_REQ* pIssuerCert = const_cast<X509_REQ*>(static_cast<const X509_REQ*>(issuerCertificate));
	EVP_PKEY* pIssuerPublicKey = X509_REQ_get_pubkey(pIssuerCert);
	if (!pIssuerPublicKey) throw Poco::InvalidArgumentException("Issuer certificate has no public key");
	int rc = X509_REQ_verify(pCert, pIssuerPublicKey);
	EVP_PKEY_free(pIssuerPublicKey);
	return rc == 1;
}


int X509Request::sign(const EVPPKey &pkey, const std::string& mdstr)
{
	const char* mdc = mdstr.c_str();
	if(std::string("default").compare(mdstr) == 0)
	{
		int defNid = 0;
		EVP_PKEY_get_default_digest_nid(const_cast<EVP_PKEY*>(static_cast<const EVP_PKEY*>(pkey)), &defNid);
		mdc = OBJ_nid2sn(defNid);
	}
	const EVP_MD* md = EVP_get_digestbyname(mdc);
	return X509_REQ_sign(_pRequest, const_cast<EVP_PKEY*>(static_cast<const EVP_PKEY*>(pkey)), md);
}

void X509Request::print(std::ostream &out) const
{
	out << "subjectName: " << subjectName() << std::endl;
	out << "commonName: " << commonName() << std::endl;
	out << "country: " << subjectName(X509Request::NID_COUNTRY) << std::endl;
	out << "localityName: " << subjectName(X509Request::NID_LOCALITY_NAME) << std::endl;
	out << "stateOrProvince: " << subjectName(X509Request::NID_STATE_OR_PROVINCE) << std::endl;
	out << "organizationName: " << subjectName(X509Request::NID_ORGANIZATION_NAME) << std::endl;
	out << "organizationUnitName: " << subjectName(X509Request::NID_ORGANIZATION_UNIT_NAME) << std::endl;
	out << "emailAddress: " << subjectName(X509Request::NID_PKCS9_EMAIL_ADDRESS) << std::endl;
	out << "serialNumber: " << subjectName(X509Request::NID_SERIAL_NUMBER) << std::endl;
}


X509_REQ* X509Request::duplicate(const X509_REQ *pFromRequest, X509_REQ **pToRequest)
{
	*pToRequest = X509_REQ_dup(const_cast<X509_REQ*>(pFromRequest));
	return *pToRequest;
}


} } // namespace Poco::Crypto
