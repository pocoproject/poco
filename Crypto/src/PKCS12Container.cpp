//
// PKCS12Container.cpp
//
//
// Library: Crypto
// Package: Certificate
// Module:  PKCS12Container
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/PKCS12Container.h"
#include "Poco/NumberFormatter.h"
#include "Poco/StreamCopier.h"
#include <sstream>
#include <openssl/err.h>


namespace Poco {
namespace Crypto {


	PKCS12Container::PKCS12Container(std::istream& istr, const std::string& password): _pKey(0)
{
	std::ostringstream ostr;
	Poco::StreamCopier::copyStream(istr, ostr);
	const std::string& cont = ostr.str();

	BIO *pBIO = BIO_new_mem_buf(const_cast<char*>(cont.data()), static_cast<int>(cont.size()));
	if (pBIO)
	{
		PKCS12* pPKCS12 = 0;
		d2i_PKCS12_bio(pBIO, &pPKCS12);
		BIO_free(pBIO);
		if (!pPKCS12) throw OpenSSLException();
		load(pPKCS12, password);
	}
	else
	{
		throw OpenSSLException();
	}
}


PKCS12Container::PKCS12Container(const std::string& path, const std::string& password): _pKey(0)
{
	FILE* pFile = fopen(path.c_str(), "rb");
	if (pFile)
	{
		PKCS12* pPKCS12 = d2i_PKCS12_fp(pFile, NULL);
		fclose (pFile);
		if (!pPKCS12) throw OpenSSLException();
		load(pPKCS12, password);
	}
	else
	{
		throw Poco::OpenFileException("PKCS12Container: " + path);
	}
}


PKCS12Container::PKCS12Container(const PKCS12Container& cont)
{
}


PKCS12Container& PKCS12Container::operator = (const PKCS12Container& cert)
{
	return *this;
}


PKCS12Container::~PKCS12Container()
{
	if (_pKey) EVP_PKEY_free(_pKey);
}


void PKCS12Container::load(PKCS12* pPKCS12, const std::string& password)
{
	if (pPKCS12)
	{
		X509* pCert = 0;
		STACK_OF(X509)* pCA = 0;
		if (PKCS12_parse(pPKCS12, password.c_str(), &_pKey, &pCert, &pCA))
		{
			if (pCert)
			{
				STACK_OF(PKCS12_SAFEBAG)* pBags = 0;
				_pX509Cert.reset(new X509Certificate(pCert, true));
				PKCS12_SAFEBAG* pBag = PKCS12_add_cert(&pBags, pCert);
				if (pBag)
				{
					char*pBuffer = PKCS12_get_friendlyname(pBag);
					if(pBuffer)
					{
						_pkcsFriendlyname = pBuffer;
						CRYPTO_free(pBuffer);
					}else _pkcsFriendlyname.clear();
					if(pBags) sk_PKCS12_SAFEBAG_pop_free(pBags, PKCS12_SAFEBAG_free);
				}
				else throw OpenSSLException();
			}
			else _pX509Cert.reset();

			_caCertList.clear();
			if (pCA)
			{
				int certCount = sk_X509_num(pCA);
				for (int i = 0; i < certCount; ++i)
				{
					_caCertList.push_back(X509Certificate(sk_X509_value(pCA, i), true));
				}
			}
		}
		else
		{
			throw OpenSSLException();
		}
		PKCS12_free(pPKCS12);
		sk_X509_pop_free(pCA, X509_free);
		X509_free(pCert);
	}
	else
	{
		throw NullPointerException("PKCS12Container: struct PKCS12");
	}
}


} } // namespace Poco::Crypto
