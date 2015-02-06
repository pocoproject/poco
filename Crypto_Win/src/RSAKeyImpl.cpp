//
// RSAKeyImpl.cpp
//
// $Id$
//
// Library: Crypto_Win
// Package: RSA
// Module:  RSAKeyImpl
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/RSAKeyImpl.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/FileStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Buffer.h"
#include "Poco/MemoryStream.h"
#include "Poco/Base64Decoder.h"
#include "Poco/Base64Encoder.h"
#include <sstream>


// TODO: this is partially implemented. PEM handling needs to be done.


namespace Poco {
namespace Crypto {


RSAKeyImpl::RSAKeyImpl(const X509Certificate& cert):
	_hPrivateKey(0),
	_hPublicKey(0)
{
	DWORD rc = CryptImportPublicKeyInfo(_sp.handle(), X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, &cert.system().pCertInfo->SubjectPublicKeyInfo, &_hPublicKey);
	if (!rc) throw Poco::SystemException("Cannot import public key from certificate");
}


RSAKeyImpl::RSAKeyImpl(int keyLength, unsigned long exponent):
	_hPrivateKey(0),
	_hPublicKey(0)
{
	DWORD flags = keyLength << 16;
	flags |= CRYPT_EXPORTABLE;
	DWORD rc = CryptGenKey(_sp.handle(), AT_SIGNATURE, flags, &_hPrivateKey);
	if (!rc) throw Poco::SystemException("Cannot generate RSA key pair");

	DWORD size = 0;
	rc = CryptExportPublicKeyInfo(_sp.handle(), AT_SIGNATURE, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, NULL, &size);
	if (rc) 
	{
		Poco::Buffer<char> keyBuffer(size);
		rc = CryptExportPublicKeyInfo(_sp.handle(), AT_SIGNATURE, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, reinterpret_cast<PCERT_PUBLIC_KEY_INFO>(keyBuffer.begin()), &size);
		if (rc)
		{
			rc = CryptImportPublicKeyInfo(_sp.handle(), X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, reinterpret_cast<PCERT_PUBLIC_KEY_INFO>(keyBuffer.begin()), &_hPublicKey);
		}
	}
	if (!rc)
	{
		CryptDestroyKey(_hPrivateKey);
		throw Poco::SystemException("Cannot extract public key");
	}
}


RSAKeyImpl::RSAKeyImpl(const std::string& publicKeyFile, const std::string& privateKeyFile, const std::string& privateKeyPassphrase):
	_hPrivateKey(0),
	_hPublicKey(0)
{
	if (!privateKeyFile.empty())
	{
		Poco::FileInputStream istr(privateKeyFile);
		loadPrivateKey(istr);
	}
	if (!publicKeyFile.empty())
	{
		Poco::FileInputStream istr(publicKeyFile);
		loadPublicKey(istr);
	}
}


RSAKeyImpl::RSAKeyImpl(std::istream* pPublicKeyStream, std::istream* pPrivateKeyStream, const std::string& privateKeyPassphrase):
	_hPrivateKey(0),
	_hPublicKey(0)
{
	if (pPrivateKeyStream)
	{
		loadPrivateKey(*pPrivateKeyStream);
	}
	if (pPublicKeyStream)
	{
		loadPublicKey(*pPublicKeyStream);
	}
}


RSAKeyImpl::~RSAKeyImpl()
{
	if (_hPrivateKey) CryptDestroyKey(_hPrivateKey);
	if (_hPublicKey) CryptDestroyKey(_hPublicKey);
}


int RSAKeyImpl::size() const
{
	DWORD keyLength;
	DWORD size = sizeof(keyLength);
	if (!CryptGetKeyParam(_hKey, KP_KEYLEN, &keyLength, &size, 0))
		throw Poco::SystemException("Cannot obtain key length");
	return static_cast<int>(keyLength);
}


void RSAKeyImpl::save(const std::string& publicKeyFile, const std::string& privateKeyFile, const std::string& privateKeyPassphrase)
{
	if (_hPrivateKey && !privateKeyFile.empty())
	{
		Poco::FileOutputStream ostr(privateKeyFile);
		savePrivateKey(ostr);
	}
	if (_hPublicKey && !publicKeyFile.empty())
	{
		Poco::FileOutputStream ostr(publicKeyFile);
		savePublicKey(ostr);
	}
}


void RSAKeyImpl::save(std::ostream* pPublicKeyStream, std::ostream* pPrivateKeyStream, const std::string& privateKeyPassphrase)
{
	if (_hPrivateKey && pPrivateKeyStream)
	{
		savePrivateKey(*pPrivateKeyStream);
	}
	if (_hPublicKey && pPublicKeyStream)
	{
		savePublicKey(*pPublicKeyStream);
	}
}


void RSAKeyImpl::loadPrivateKey(std::istream& istr)
{
	std::string data;
	std::string der;
	Poco::StreamCopier::copyToString(istr, data);
	if (data.compare(0, 31, "-----BEGIN RSA PRIVATE KEY-----") == 0)
	{
		const char* pemBegin = data.data() + 31;
		const char* pemEnd = data.data() + data.size() - 29;
		while (pemEnd > pemBegin && std::memcmp(pemEnd, "-----END RSA PRIVATE KEY-----", 29) != 0) --pemEnd;
		if (pemEnd == pemBegin) throw Poco::DataFormatException("Not a valid PEM file - end marker missing");

		Poco::MemoryInputStream mis(pemBegin, pemEnd - pemBegin);
		Poco::Base64Decoder dec(mis);
		Poco::StreamCopier::copyToString(dec, der);
	}

}


void RSAKeyImpl::loadPublicKey(std::istream& istr)
{
}


void RSAKeyImpl::savePrivateKey(std::ostream& ostr)
{
}


void RSAKeyImpl::savePublicKey(std::ostream& ostr)
{
}


} } // namespace Poco::Crypto
