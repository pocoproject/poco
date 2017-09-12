//
// PKCS12Container.cpp
//
// $Id: //poco/1.4/Crypto/src/PKCS12Container.cpp#1 $
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


PKCS12Container::PKCS12Container(std::istream& istr, const std::string& password)
{
	std::ostringstream ostr;
	Poco::StreamCopier::copyStream(istr, ostr);
	const std::string& cont = ostr.str();

	BIO *pBIO = BIO_new_mem_buf(const_cast<char*>(cont.c_str()), static_cast<int>(cont.size()));
	if (pBIO)
	{
		PKCS12* pPKCS12 = d2i_PKCS12_bio(pBIO, NULL);
		BIO_free(pBIO);
		if (!pPKCS12) throw OpenSSLException();
		load(pPKCS12, password);
	}
	else
	{
		throw NullPointerException("PKCS12Container BIO memory buffer");
	}
}


PKCS12Container::PKCS12Container(const Poco::Path& path, const std::string& password)
{
	FILE* pFile = nullptr;

	if ((pFile = fopen(path.toString().c_str(), "rb")))
	{
		PKCS12* pPKCS12 = d2i_PKCS12_fp(pFile, NULL);
		fclose (pFile);
		if (!pPKCS12) throw OpenSSLException();
		load(pPKCS12, password);
	}
	else
	{
		throw NullPointerException("PKCS12Container file:" + path.toString());
	}
}


PKCS12Container::PKCS12Container(const std::string& str, const std::string& password)
{
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
		X509* pCert = nullptr;
		STACK_OF(X509)* pCA = nullptr;
		if (PKCS12_parse(pPKCS12, password.c_str(), &_pKey, &pCert, &pCA))
		{
			if (pCert)
			{
				STACK_OF(PKCS12_SAFEBAG)* bags = nullptr;
				_pX509Cert.reset(new X509Certificate(pCert));
				PKCS12_SAFEBAG* bag = PKCS12_add_cert(&bags, pCert);
				char* buffer = PKCS12_get_friendlyname(bag);
				if (buffer) _pkcsFriendlyname = buffer;
				else _pkcsFriendlyname.clear();
			}
			else _pX509Cert.reset();

			_caCertList.clear();
			if (pCA)
			{
				int certCount = sk_X509_num(pCA);
				for (int i = 0; i < certCount; ++i)
				{
					_caCertList.push_back(X509Certificate(sk_X509_value(pCA, i)));
				}
			}
		}
		else
		{
			throw OpenSSLException();
		}
		PKCS12_free(pPKCS12);
		// ??? TODO:
		// crashing here, not sure if these should be freed
		// ************************
		// sk_X509_pop_free(pCA, X509_free);
		// X509_free(pCert);
		// *************************
		// there's an example on SO with these calls, however: 
		// https://github.com/openssl/openssl/blob/master/demos/pkcs12/pkread.c
		// and 
		// https://opensource.apple.com/source/OpenSSL/OpenSSL-46/openssl/doc/openssl.txt (3.1 Parsing with PKCS12_parse().)
		//
		// !!! check with valgrind
	}
	else
	{
		throw NullPointerException("PKCS12Container: struct PKCS12");
	}
}


} } // namespace Poco::Crypto
