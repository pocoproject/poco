//
// X509Extension.cpp
//
// $Id: $
//
// Library: Crypto
// Package: Certificate
// Module:  X509Extension
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/X509Extension.h"
#include <openssl/x509v3.h>


namespace Poco {
namespace Crypto {


X509Extension X509Extension::createWithBasicConstraints(X509Extension::BASIC_CONTRAINTS type)
{
	unsigned char data[] = {0x30, 0x03, 0x01, 0x01, 0xff};
	bool isCritical = true;
	switch(type)
	{
	case CRITICAL_CA_TRUE:
		break;
	case CRITICAL_CA_FALSE:
		break;
	case CA_TRUE:
		isCritical = false;
		break;
	case CA_FALSE:
		isCritical = false;
		break;
	}
	size_t lenght = sizeof (data);

	return create(NID_basic_constraints, isCritical, data, lenght);
}


X509Extension X509Extension::create(int nid, const std::string& value)
{
	CONF* conf = nullptr;
	X509V3_CTX *ctx = nullptr;
	X509_EXTENSION* ext = X509V3_EXT_nconf_nid(conf, ctx, nid, const_cast<char*>(value.c_str()));
	return X509Extension(ext);
}


X509Extension X509Extension::create(X509V3_CTX *ctx, int nid, const std::string& value)
{
	CONF* conf = nullptr;
	X509_EXTENSION* ext = X509V3_EXT_nconf_nid(conf, ctx, nid, const_cast<char*>(value.c_str()));
	return X509Extension(ext);
}


X509Extension X509Extension::create(int nid, bool critical, void* data, size_t lenght)
{
	ASN1_STRING* asn1 = ASN1_STRING_new();
	ASN1_STRING_set(asn1, data, static_cast<int>(lenght));
	int crit = critical ? 1 : 0;
	X509_EXTENSION **ex = nullptr;
	X509_EXTENSION *ext = X509_EXTENSION_create_by_NID(ex, nid, crit, asn1);
	ASN1_STRING_free(asn1);
	return X509Extension(ext);
}


X509Extension::X509Extension(X509_EXTENSION* pExt):
	_pExt(pExt)
{

}


X509Extension::X509Extension(const X509Extension &other):
	_pExt(nullptr)
{
	duplicate(other._pExt, &_pExt);
}


X509Extension::X509Extension(X509Extension&& other):
	_pExt(nullptr)
{
	swap(*this, other);
}


X509Extension &X509Extension::operator =(X509Extension other)
{
	swap(*this, other);
	return *this;
}


void X509Extension::swap(X509Extension& first, X509Extension& second)
{
	using std::swap;
	swap(first._pExt, second._pExt);
}


X509Extension::~X509Extension()
{
	X509_EXTENSION_free(_pExt);
}


bool X509Extension::isCritical() const
{
	return (X509_EXTENSION_get_critical(_pExt) != 0);
}


int X509Extension::nid() const
{
	ASN1_OBJECT* asn1object = X509_EXTENSION_get_object(_pExt);
	return OBJ_obj2nid(asn1object);
}


const unsigned char* X509Extension::data() const
{
	ASN1_OCTET_STRING* asn1 = X509_EXTENSION_get_data(_pExt);
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
	return ASN1_STRING_get0_data(asn1);
#else
	return ASN1_STRING_data(asn1);
#endif
}


size_t X509Extension::size() const
{
	ASN1_OCTET_STRING* asn1 = X509_EXTENSION_get_data(_pExt);
	return static_cast<size_t>(ASN1_STRING_length(asn1));
}


std::string X509Extension::str() const
{
	return std::string(reinterpret_cast<const char*>(data()), size());
}


X509_EXTENSION* X509Extension::duplicate(const X509_EXTENSION *pFromExtension, X509_EXTENSION **pToExtension)
{
	*pToExtension = X509_EXTENSION_dup(const_cast<X509_EXTENSION*>(pFromExtension));
	return *pToExtension;
}


} } // namespace Poco::Crypto
