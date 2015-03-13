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
#include "Poco/NumericString.h"
#include "Poco/StringTokenizer.h"
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
	if (!CryptGenKey(_sp.handle(), AT_KEYEXCHANGE, flags, &_hPrivateKey))
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
		AT_KEYEXCHANGE, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		NULL, &size);
	if (rc)
	{
		keyBuffer.resize(size);
		rc = CryptExportPublicKeyInfo(_sp.handle(),
			AT_KEYEXCHANGE, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
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
	Poco::Buffer<char> derBuffer(der.data(), der.size());

	Poco::StreamCopier::copyToString(istr, pem);
	
	if (pem.compare(0, beginLen, BEGIN_RSA_PRIVATE) == 0)
	{
		if (!privateKeyPassphrase.empty())
		{
			std::istringstream istr(pem);
			std::string line;
			while (std::getline(istr, line, '\n'))
			{
				if (line.substr(0, beginLen) == BEGIN_RSA_PRIVATE) continue;
				else if (line.substr(0, 10) == "Proc-Type:")
				{
					StringTokenizer st(line, ":,\r\n",
						StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);
					if (st[1] != "4" || st[2] != "ENCRYPTED") break;
					beginLen += line.size();
					if (line.find('\r') != std::string::npos) // \r\n
					{
						beginLen += std::count(line.begin(), line.end(), '\r');
						++beginLen;
					}
					else // \n
						++beginLen;
				}
				else if (line.substr(0, 9) == "DEK-Info:")
				{
					StringTokenizer st(line, ":,\r\n",
						StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);
					poco_assert(st[2].size() >= 3);
					if (st[1] != "DES-EDE3-CBC")
					{
						throw Poco::InvalidArgumentException(
							Poco::format("%s encryption not supported.", st[1]));
					}
					// initialization vector
					poco_assert(st[2].size() % 2 == 0);
					for (std::string::size_type i = 0; i < st[2].size(); i += 2)
					{
						BYTE num;
						Poco::strToInt(st[2].substr(i, 2), num, 0x10);
						_initVector.append(num);
					}
					beginLen += line.size();
					if (line.find('\r') != std::string::npos)
					{
						beginLen += std::count(line.begin(), line.end(), '\r');
						++beginLen;
					}
					else
						++beginLen;
				}
				else if (line == "\r") // "\r\n" empty line
				{
					beginLen += std::count(line.begin(), line.end(), '\r');
					++beginLen;
					break;
				}
				else if (line.empty()) // "\n" empty line
				{
					++beginLen;
					break;
				}
				else break;
			}
		}
		const char* pemBegin = pem.data() + beginLen;
		const char* pemEnd = pem.data() + pem.size() - endLen;
		while (pemEnd > pemBegin && std::memcmp(pemEnd, END_RSA_PRIVATE, endLen) != 0)
			--pemEnd;

		if (pemEnd == pemBegin)
			throw Poco::DataFormatException("Not a valid PEM file - end marker missing");

		Poco::MemoryInputStream mis(pemBegin, pemEnd - pemBegin);
		Poco::Base64Decoder dec(mis);
		Poco::StreamCopier::copyToString(dec, der);

		derBuffer.assign(der.data(), der.size());
		if (!privateKeyPassphrase.empty())
		{
			crypt(derBuffer, privateKeyPassphrase, CRYPT_DIR_DECRYPT);
		}
	}
	else
		throw Poco::DataFormatException("Private key beginning string not found");

	DWORD size = 0;
	if (!CryptDecodeObjectEx(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, PKCS_RSA_PRIVATE_KEY,
		reinterpret_cast<LPBYTE>(const_cast<char*>(derBuffer.begin())),
		static_cast<DWORD>(derBuffer.size()), 0, NULL, NULL, &size))
	{
		error("Failed to obtain length of data needed to decode private key");
	}

	Poco::Buffer<char> keyBuffer(size);
	if (!CryptDecodeObjectEx(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, PKCS_RSA_PRIVATE_KEY,
		reinterpret_cast<LPBYTE>(const_cast<char*>(derBuffer.begin())),
		static_cast<DWORD>(derBuffer.size()), 0, NULL, keyBuffer.begin(), &size))
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

		if (!privateKeyPassphrase.empty())
		{
			crypt(derBuffer, privateKeyPassphrase, CRYPT_DIR_ENCRYPT);
			if (_initVector.size() == 0)
			{
				throw Poco::IllegalStateException(
					"Cannot encrypt private key without initialization vector.");
			}

			ostr << BEGIN_RSA_PRIVATE << "\r\n";
			ostr << "Proc-Type: 4,ENCRYPTED\r\n";
			std::string initVec;
			std::string chr;
			for (std::size_t i = 0; i < _initVector.size(); ++i)
			{
				Poco::uIntToStr(_initVector[i], 0x10, chr, false, 2, '0');
				initVec.append(chr);
			}
			ostr << "DEK-Info: DES-EDE3-CBC," << initVec << "\r\n";
			ostr << "\r\n";
			std::ostringstream os;
			Poco::Base64Encoder enc(os);
			enc.rdbuf()->setLineLength(64);
			enc << std::string(derBuffer.begin(), derBuffer.size()) << std::flush;
			enc.close();
			ostr << os.str() << "\r\n" << END_RSA_PRIVATE << std::flush;
		}
		else
		{
			ostr << BEGIN_RSA_PRIVATE << "\r\n";
			std::ostringstream os;
			Poco::Base64Encoder enc(os);
			enc.rdbuf()->setLineLength(64);
			enc << std::string(derBuffer.begin(), derBuffer.size()) << std::flush;
			enc.close();
			ostr << os.str() << "\r\n" << END_RSA_PRIVATE << std::flush;
		}
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


void RSAKeyImpl::makeIV(HCRYPTKEY hKey)
{
	DWORD dwBlockLen = 0;
	DWORD dwDataLen = sizeof(dwBlockLen);

	if (!CryptGetKeyParam(hKey, KP_BLOCKLEN, reinterpret_cast<LPBYTE>(&dwBlockLen), &dwDataLen, 0))
	{
		error("Cannot assign block length parameter to key");
	}

	dwBlockLen /= 8;
	_initVector.resize(dwBlockLen, false);

	if (!CryptGenRandom(_sp.handle(), dwBlockLen, _initVector.begin()))
	{
		error("Cannot assign block length parameter to key");
	}
}


void RSAKeyImpl::crypt(Poco::Buffer<char>& data, const std::string& privateKeyPassphrase, CryptDirection dir)
{
	if (privateKeyPassphrase.empty())
		throw Poco::InvalidAccessException("Cannot encrypt private key with empty password");

	HCRYPTKEY  hSecretKey = 0;
	HCRYPTHASH hHash = 0;
	try
	{
		BOOL rc = CryptCreateHash(_sp.handle(), CALG_SHA1, 0, 0, &hHash);

		if (rc)
		{
			rc = CryptHashData(hHash,
				reinterpret_cast<LPBYTE>(const_cast<char*>(privateKeyPassphrase.c_str())),
				static_cast<DWORD>(privateKeyPassphrase.size()), 0);

			if (rc)
			{
				rc = CryptDeriveKey(_sp.handle(), CALG_3DES,
					hHash, 0, &hSecretKey);
				if (rc)
				{
					DWORD dwMode = CRYPT_MODE_CBC;
					rc = CryptSetKeyParam(hSecretKey, KP_MODE, reinterpret_cast<LPBYTE>(&dwMode), 0);
					if (rc)
					{
						DWORD size = static_cast<DWORD>(data.size());

						if (dir == CRYPT_DIR_ENCRYPT)
						{
							if (_initVector.size() == 0) makeIV(hSecretKey);

							rc = CryptSetKeyParam(hSecretKey, KP_IV, _initVector.begin(), 0);
							if (rc)
							{
								rc = CryptEncrypt(hSecretKey, NULL, TRUE, 0, NULL, &size, 0);
								if (rc)
								{
									DWORD plainSize = static_cast<DWORD>(data.size());
									if (size != data.size()) data.resize(size, true);
									rc = CryptEncrypt(hSecretKey, NULL, TRUE, 0,
										reinterpret_cast<LPBYTE>(data.begin()), &plainSize,
										static_cast<DWORD>(data.size()));
								}
							}
						}
						else if (dir == CRYPT_DIR_DECRYPT)
						{
							if (!_initVector.size())
							{
								throw Poco::IllegalStateException(
									"Cannot decrypt private key without initialization vector.");
							}

							rc = CryptSetKeyParam(hSecretKey, KP_IV, _initVector.begin(), 0);
							if (rc)
							{
								rc = CryptDecrypt(hSecretKey, NULL, TRUE, 0,
								reinterpret_cast<LPBYTE>(data.begin()), &size);
								if (size != data.size()) data.resize(size, true);
							}
						}
						else
							throw Poco::InvalidAccessException("Bad crypt direction");
					}
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
