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
#include "Poco/MemoryStream.h"
#include "Poco/Base64Decoder.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Format.h"
#include "Poco/String.h"
#include <sstream>


namespace Poco {
namespace Crypto {


const char* RSAKeyImpl::BEGIN_RSA_PRIVATE = "-----BEGIN RSA PRIVATE KEY-----";
const char* RSAKeyImpl::END_RSA_PRIVATE = "-----END RSA PRIVATE KEY-----";
const char* RSAKeyImpl::BEGIN_RSA_PUBLIC = "-----BEGIN RSA PUBLIC KEY-----";
const char* RSAKeyImpl::END_RSA_PUBLIC = "-----END RSA PUBLIC KEY-----";


RSAKeyImpl::RSAKeyImpl(const X509Certificate& cert):
	_hPrivateKey(0),
	_hPublicKey(0)
{
	if (!CryptImportPublicKeyInfo(_sp.handle(), X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		&cert.system()->pCertInfo->SubjectPublicKeyInfo, &_hPublicKey))
	{
		error("Cannot import public key from certificate");
	}
}


RSAKeyImpl::RSAKeyImpl(int keyLength, unsigned long):
	_hPrivateKey(0),
	_hPublicKey(0)
{
	DWORD flags = keyLength << 16;
	flags |= CRYPT_EXPORTABLE;
	if (!CryptGenKey(_sp.handle(), AT_SIGNATURE, flags, &_hPrivateKey))
	{
		error("Cannot generate RSA key pair");
	}

	try
	{
		extractPublicKey();
	}
	catch (std::exception&)
	{
		CryptDestroyKey(_hPrivateKey);
		throw;
	}
}


RSAKeyImpl::RSAKeyImpl(const std::string& publicKeyFile, const std::string& privateKeyFile, const std::string& privateKeyPassphrase):
	_hPrivateKey(0),
	_hPublicKey(0)
{
	if (!privateKeyFile.empty())
	{
		Poco::FileInputStream istr(privateKeyFile);
		loadPrivateKey(istr, privateKeyPassphrase);
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
		loadPrivateKey(*pPrivateKeyStream, privateKeyPassphrase);
		// TODO: load public from private when pPublicKeyStream is null
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


void RSAKeyImpl::extractPublicKey()
{
	Poco::Buffer<char> keyBuffer;

	DWORD size = 0;
	DWORD rc = CryptExportPublicKeyInfo(_sp.handle(),
		AT_SIGNATURE, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		NULL, &size);
	if (rc)
	{
		keyBuffer.resize(size);
		rc = CryptExportPublicKeyInfo(_sp.handle(),
			AT_SIGNATURE, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
			reinterpret_cast<PCERT_PUBLIC_KEY_INFO>(keyBuffer.begin()), &size);
		if (rc)
		{
			rc = CryptImportPublicKeyInfo(_sp.handle(),
				X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
				reinterpret_cast<PCERT_PUBLIC_KEY_INFO>(keyBuffer.begin()), &_hPublicKey);
		}
	}
	if (!rc)
	{
		error("Cannot extract public key");
	}
}


void RSAKeyImpl::importPublicKey(Poco::Buffer<char>& keyBuffer)
{
	if (!CryptImportKey(_sp.handle(), reinterpret_cast<BYTE*>(keyBuffer.begin()),
		static_cast<DWORD>(keyBuffer.size()), 0, CRYPT_EXPORTABLE, &_hPublicKey))
	{
		error("Cannot export public key");
	}
}


int RSAKeyImpl::size() const
{
	DWORD keyLength = 0;
	DWORD size = sizeof(DWORD);
	if (CryptGetKeyParam(_hPrivateKey, KP_KEYLEN, reinterpret_cast<BYTE*>(&keyLength), &size, 0))
	{
		keyLength /= 8;
	}
	else
	{
		error("Cannot obtain key length");
	}

	return static_cast<int>(keyLength);
}


void RSAKeyImpl::loadPrivateKey(std::istream& istr, const std::string& privateKeyPassphrase)
{
	std::string pem;
	std::string der;
	std::size_t beginLen = std::strlen(BEGIN_RSA_PRIVATE);
	std::size_t endLen = std::strlen(END_RSA_PRIVATE);

	Poco::StreamCopier::copyToString(istr, pem);
	
	if (pem.compare(0, beginLen, BEGIN_RSA_PRIVATE) == 0)
	{
		const char* pemBegin = pem.data() + beginLen;
		const char* pemEnd = pem.data() + pem.size() - endLen;
		while (pemEnd > pemBegin && std::memcmp(pemEnd, END_RSA_PRIVATE, endLen) != 0)
			--pemEnd;

		if (pemEnd == pemBegin)
			throw Poco::DataFormatException("Not a valid PEM file - end marker missing");

		Poco::Buffer<char> pemBuffer(pemBegin, pemEnd - pemBegin);
		if (!privateKeyPassphrase.empty())
		{
			crypt(pemBuffer, privateKeyPassphrase, CRYPT_DIR_DECRYPT);
		}

		Poco::MemoryInputStream mis(pemBuffer.begin(), pemBuffer.size());
		Poco::Base64Decoder dec(mis);
		Poco::StreamCopier::copyToString(dec, der);
	}

	DWORD size = 0;
	if (!CryptDecodeObjectEx(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, PKCS_RSA_PRIVATE_KEY,
		reinterpret_cast<LPBYTE>(const_cast<char*>(der.data())),
		static_cast<DWORD>(der.size()), 0, NULL, NULL, &size))
	{
		error("Failed to obtain length of data needed to decode private key");
	}

	Poco::Buffer<char> keyBuffer(size);
	if (!CryptDecodeObjectEx(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, PKCS_RSA_PRIVATE_KEY,
		reinterpret_cast<LPBYTE>(const_cast<char*>(der.data())),
		static_cast<DWORD>(der.size()), 0, NULL, keyBuffer.begin(), &size))
	{
		error("Failed to decode private key");
	}

	if (!CryptImportKey(_sp.handle(), reinterpret_cast<LPBYTE>(keyBuffer.begin()),
		static_cast<DWORD>(keyBuffer.size()), 0, CRYPT_EXPORTABLE, &_hPrivateKey))
	{
		error("Cannot load private key");
	}
}


void RSAKeyImpl::loadPublicKey(std::istream& istr)
{
	std::string pem;
	std::string der;

	std::size_t beginLen = std::strlen(BEGIN_RSA_PUBLIC);
	std::size_t endLen = std::strlen(END_RSA_PUBLIC);

	Poco::StreamCopier::copyToString(istr, pem);
	if (pem.compare(0, beginLen, BEGIN_RSA_PUBLIC) == 0)
	{
		const char* pemBegin = pem.data() + beginLen;
		const char* pemEnd = pem.data() + pem.size() - endLen;
		while (pemEnd > pemBegin && std::memcmp(pemEnd, END_RSA_PUBLIC, endLen) != 0)
			--pemEnd;

		if (pemEnd == pemBegin)
			throw Poco::DataFormatException("Not a valid PEM file - end marker missing");

		Poco::MemoryInputStream mis(pemBegin, pemEnd - pemBegin);
		Poco::Base64Decoder dec(mis);
		Poco::StreamCopier::copyToString(dec, der);
	}

	DWORD size = 0;
	if (!CryptDecodeObjectEx(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, RSA_CSP_PUBLICKEYBLOB,
		reinterpret_cast<LPBYTE>(const_cast<char*>(der.data())),
		static_cast<DWORD>(der.size()), 0, NULL, NULL, &size))
	{
		error("Failed to obtain length of data needed to decode public key");
	}

	Poco::Buffer<char> keyBuffer(size);
	if (!CryptDecodeObjectEx(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, RSA_CSP_PUBLICKEYBLOB,
		reinterpret_cast<LPBYTE>(const_cast<char*>(der.data())),
		static_cast<DWORD>(der.size()), 0, NULL, keyBuffer.begin(), &size))
	{
		error("Failed to decode public key");
	}

	if (!_hPublicKey && _hPrivateKey)
	{
		exportKey(_hPrivateKey, PUBLICKEYBLOB, keyBuffer);
	}

	if (!CryptImportKey(_sp.handle(), reinterpret_cast<LPBYTE>(keyBuffer.begin()),
		static_cast<DWORD>(keyBuffer.size()), 0, CRYPT_EXPORTABLE, &_hPublicKey))
	{
		error("Cannot load public key");
	}
}


void RSAKeyImpl::save(const std::string& publicKeyFile,
	const std::string& privateKeyFile,
	const std::string& privateKeyPassphrase)
{
	if (!privateKeyFile.empty())
	{
		Poco::FileOutputStream ostr(privateKeyFile);
		savePrivateKey(ostr, privateKeyPassphrase);
	}

	if (!publicKeyFile.empty())
	{
		Poco::FileOutputStream ostr(publicKeyFile);
		savePublicKey(ostr);
	}
}


void RSAKeyImpl::save(std::ostream* pPublicKeyStream,
	std::ostream* pPrivateKeyStream,
	const std::string& privateKeyPassphrase)
{
	if (pPrivateKeyStream)
		savePrivateKey(*pPrivateKeyStream, privateKeyPassphrase);

	if (pPublicKeyStream)
		savePublicKey(*pPublicKeyStream);
}


void RSAKeyImpl::exportKey(HCRYPTKEY hKey, int type, Poco::Buffer<char>& keyBuffer)
{
	DWORD size = 0;
	DWORD rc = CryptExportKey(hKey, 0, type, 0, 0, &size);
	if (rc)
	{
		keyBuffer.resize(size, false);
		rc = CryptExportKey(hKey, 0, type, 0,
			reinterpret_cast<BYTE*>(keyBuffer.begin()), &size);
	}

	if (!rc)
	{
		error("Cannot export private key");
	}
}


void RSAKeyImpl::savePrivateKey(std::ostream& ostr, const std::string& privateKeyPassphrase)
{
	if (_hPrivateKey)
	{
		Poco::Buffer<char> keyBuffer;
		exportKey(_hPrivateKey, PRIVATEKEYBLOB, keyBuffer);

		DWORD size = 0;
		if (!CryptEncodeObjectEx(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
			PKCS_RSA_PRIVATE_KEY, keyBuffer.begin(), 0, NULL, NULL, &size))
		{
			error("Failed to obtain length of data needed to encode private key");
		}

		Poco::Buffer<char> derBuffer(size);
		if (!CryptEncodeObjectEx(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
			PKCS_RSA_PRIVATE_KEY, keyBuffer.begin(), 0, NULL, derBuffer.begin(), &size))
		{
			error("Failed to encode private key");
		}

		ostr << BEGIN_RSA_PRIVATE;
		std::ostringstream os;
		Poco::Base64Encoder enc(os);
		enc.rdbuf()->setLineLength(64);
		enc << std::string(derBuffer.begin(), derBuffer.size()) << std::flush;
		enc.close();

		if (!privateKeyPassphrase.empty())
		{
			std::string encPEM(os.str());
			keyBuffer.assign(encPEM.c_str(), encPEM.size());
			crypt(keyBuffer, privateKeyPassphrase, CRYPT_DIR_ENCRYPT);
			ostr.write(keyBuffer.begin(), keyBuffer.size());
		}
		else
		{
			ostr << os.str();
		}

		ostr << END_RSA_PRIVATE << std::flush;
	}
	else
		throw Poco::IllegalStateException("No private key.");
}


void RSAKeyImpl::savePublicKey(std::ostream& ostr)
{
	Poco::Buffer<char> keyBuffer;
	if (_hPublicKey)
		exportKey(_hPublicKey, PUBLICKEYBLOB, keyBuffer);
	else if (_hPrivateKey)
		exportKey(_hPrivateKey, PUBLICKEYBLOB, keyBuffer);
	else
	throw Poco::IllegalStateException("No public key.");

	DWORD size = 0;
	if (!CryptEncodeObjectEx(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		RSA_CSP_PUBLICKEYBLOB, keyBuffer.begin(), 0, NULL, NULL, &size))
	{
		error("Failed to obtain length of data needed to encode public key");
	}

	Poco::Buffer<char> derBuffer(size);
	if (!CryptEncodeObjectEx(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		RSA_CSP_PUBLICKEYBLOB, keyBuffer.begin(), 0, NULL, derBuffer.begin(), &size))
	{
		error("Failed to encode public key");
	}

	ostr << BEGIN_RSA_PUBLIC;
	std::ostringstream os;
	Poco::Base64Encoder enc(os);
	enc.rdbuf()->setLineLength(64);
	enc << std::string(derBuffer.begin(), derBuffer.size());
	enc.close();
	ostr << os.str() << END_RSA_PUBLIC << std::flush;
}


void RSAKeyImpl::crypt(Poco::Buffer<char>& data, const std::string& privateKeyPassphrase, CryptDirection dir)
{
	if (privateKeyPassphrase.empty())
		throw Poco::InvalidAccessException("Cannot encrypt with empty password");

	HCRYPTKEY  hSecretKey = 0;
	HCRYPTHASH hHash = 0;
	try
	{
		DWORD rc = CryptCreateHash(_sp.handle(), CALG_MD5, 0, 0, &hHash);

		if (rc)
		{
			rc = CryptHashData(hHash,
				reinterpret_cast<LPBYTE>(const_cast<char*>(privateKeyPassphrase.c_str())),
				static_cast<DWORD>(privateKeyPassphrase.size()), 0);

			if (rc)
			{
				rc = CryptDeriveKey(_sp.handle(), CALG_RC4,
					hHash, 0x00800000, &hSecretKey);

				if (rc)
				{
					DWORD size = static_cast<DWORD>(data.size());

					if (dir == CRYPT_DIR_ENCRYPT)
					{
						rc = CryptEncrypt(hSecretKey, NULL, TRUE, 0,
							reinterpret_cast<LPBYTE>(data.begin()), &size,
							static_cast<DWORD>(data.size()));
					}
					else if (dir == CRYPT_DIR_DECRYPT)
					{
						rc = CryptDecrypt(hSecretKey, NULL, TRUE, 0,
							reinterpret_cast<LPBYTE>(data.begin()), &size);
					}
					else
						throw Poco::InvalidAccessException("Bad crypt direction");

					poco_assert_dbg(size <= data.size());
				}
			}
		}

		if (!rc)
		{
			error("Cannot encrypt private key");
		}
	}
	catch (std::exception&)
	{
		if (hHash) CryptDestroyHash(hHash);
		if (hSecretKey) CryptDestroyKey(hSecretKey);
		throw;
	}

	if (hHash) CryptDestroyHash(hHash);
	if (hSecretKey) CryptDestroyKey(hSecretKey);
}


} } // namespace Poco::Crypto
