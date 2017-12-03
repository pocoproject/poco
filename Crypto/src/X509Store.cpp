//
// X509Store.cpp
//
// $Id: $
// Library: Crypto
// Package: Certificate
// Module:  X509Engine
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/X509Store.h"
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/buffer.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <cstring>


namespace Poco {
namespace Crypto {


X509Store::X509Store():
	_pStore(X509_STORE_new()),
	_purpose(0)
{

}


X509Store::X509Store(X509_STORE* store):
	_pStore(store)
{
	poco_check_ptr(_pStore);
}


X509Store::X509Store(X509Store&& other):
	_pStore(nullptr)
{
	swap(*this, other);
}


void X509Store::swap(X509Store& first, X509Store& second)
{
	using std::swap;

	swap(first._pStore, second._pStore);
}


X509Store::~X509Store()
{
	X509_STORE_free(_pStore);
}


void X509Store::setX509Purpose(int purpose)
{
	_purpose = purpose;
}


void X509Store::setX509VerifyFlags(unsigned long flags)
{
	X509_STORE_set_flags(_pStore, flags);
}


bool X509Store::addCa(const X509Certificate& caCertificate)
{
	return X509_STORE_add_cert(_pStore, const_cast<X509*>(static_cast<const X509*>(caCertificate)));
}

bool X509Store::addChain(const X509Certificate::List &chain)
{
	bool result = false;
	for(size_t x = 0; x < chain.size(); ++x)
	{
		result = X509_STORE_add_cert(_pStore, const_cast<X509*>(static_cast<const X509*>(chain[x])));
	}
	return result;
}


bool X509Store::addCrl(const X509RevocationList& crl)
{
	return X509_STORE_add_crl(_pStore, const_cast<X509_CRL*>(static_cast<const X509_CRL*>(crl)));
}


X509Store::VerifyResult X509Store::verifyCertificateChain(const X509Certificate& cert)
{
	X509_STORE_set_flags(_pStore, 0);
	X509_STORE_CTX *csc = X509_STORE_CTX_new();
	X509_STORE_CTX_init(csc,_pStore,const_cast<X509*>(static_cast<const X509*>(cert)),nullptr);
	if (_purpose)
	{
		X509_STORE_CTX_set_purpose(csc, _purpose);
	}
	VerifyResult result;
	int rc = X509_verify_cert(csc);
	if (rc != 1)
	{
		int code = X509_STORE_CTX_get_error(csc);
		int depth = X509_STORE_CTX_get_error_depth(csc);
		const char* errorChar = X509_verify_cert_error_string(code);
		std::string errorString(errorChar);
		X509* x509cert = X509_STORE_CTX_get_current_cert(csc);
		x509cert = X509_dup(x509cert);
		X509Certificate certificate;
		if(x509cert)
		{
			certificate = X509Certificate(x509cert);
		}
		result = VerifyResult(certificate, depth, code, errorString);
	}
	X509_STORE_CTX_free(csc);
	return result;
}


X509Store::VerifyResult::VerifyResult() :
	_certificate(),
	_depth(-1),
	_code(X509_V_OK),
	_errorString("Verification of certification chain was successful")
{
}

X509Store::VerifyResult::VerifyResult(const X509Certificate &certificate, int depth, int code, std::string errorString) :
	_certificate(certificate),
	_depth(depth),
	_code(code),
	_errorString(errorString)
{

}


} // namespace Crypto
} // namespace Poco
