//
// CipherKeyImpl.cpp
//
// $Id$
//
// Library: Crypto_Win
// Package: Cipher
// Module:  CipherKeyImpl
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/CipherKeyImpl.h"
#include "Poco/Crypto/CryptoTransform.h"
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/DigestEngine.h"
#include "Poco/Exception.h"
#include "Poco/RandomStream.h"
#include "Poco/String.h"


namespace
{
	// TODO: Not sure if we need this. May be useful for importing PEM private/public keys. Delete if not needed

	// Following code taken and adapted from http://support.microsoft.com/kb/228786.
	//
	// Sometimes it is convenient to export and import plain text session keys. 
	// However, the Microsoft Cryptographic Providers (Base and Enhanced) do not 
	// support this feature. Both CryptExportKey() and CryptImportKey() require a 
	// valid key handle to encrypt and decrypt the session key, respectively. 
	// But by using an "exponent-of-one" private key the same effect can be achieved 
	// to encrypt and decrypt the session key.

	BOOL CreatePrivateExponentOneKey(HCRYPTPROV hProv,
		DWORD dwKeySpec,
		HCRYPTKEY *hPrivateKey);

	BOOL GenerateSessionKeyWithAlgorithm(HCRYPTPROV hProv,
		ALG_ID Alg,
		HCRYPTKEY *hSessionKey);

	BOOL DeriveSessionKeyWithAlgorithm(HCRYPTPROV hProv,
		ALG_ID Alg,
		LPBYTE lpHashingData,
		DWORD dwHashingData,
		HCRYPTKEY *hSessionKey);

	BOOL ExportPlainSessionBlob(HCRYPTKEY hPublicKey,
		HCRYPTKEY hSessionKey,
		LPBYTE *pbKeyMaterial,
		DWORD *dwKeyMaterial);

	BOOL ImportPlainSessionBlob(HCRYPTPROV hProv,
		HCRYPTKEY hPrivateKey,
		ALG_ID dwAlgId,
		LPBYTE pbKeyMaterial,
		DWORD dwKeyMaterial,
		HCRYPTKEY *hSessionKey);


	BOOL CreatePrivateExponentOneKey(HCRYPTPROV hProv,
		DWORD dwKeySpec,
		HCRYPTKEY *hPrivateKey)
	{
		BOOL fReturn = FALSE;
		BOOL fResult;
		int n;
		LPBYTE keyblob = NULL;
		DWORD dwkeyblob;
		DWORD dwBitLen;
		BYTE *ptr;

		__try
		{
			*hPrivateKey = 0;

			if ((dwKeySpec != AT_KEYEXCHANGE) && (dwKeySpec != AT_SIGNATURE)) __leave;

			// Generate the private key
			fResult = CryptGenKey(hProv, dwKeySpec, CRYPT_EXPORTABLE, hPrivateKey);
			if (!fResult) __leave;

			// Export the private key, we'll convert it to a private
			// exponent of one key
			fResult = CryptExportKey(*hPrivateKey, 0, PRIVATEKEYBLOB, 0, NULL, &dwkeyblob);
			if (!fResult) __leave;

			keyblob = (LPBYTE)LocalAlloc(LPTR, dwkeyblob);
			if (!keyblob) __leave;

			fResult = CryptExportKey(*hPrivateKey, 0, PRIVATEKEYBLOB, 0, keyblob, &dwkeyblob);
			if (!fResult) __leave;

			CryptDestroyKey(*hPrivateKey);
			*hPrivateKey = 0;

			// Get the bit length of the key
			memcpy(&dwBitLen, &keyblob[12], 4);

			// Modify the Exponent in Key BLOB format
			// Key BLOB format is documented in SDK

			// Convert pubexp in rsapubkey to 1
			ptr = &keyblob[16];
			for (n = 0; n < 4; n++)
			{
				if (n == 0) ptr[n] = 1;
				else ptr[n] = 0;
			}

			// Skip pubexp
			ptr += 4;
			// Skip modulus, prime1, prime2
			ptr += (dwBitLen / 8);
			ptr += (dwBitLen / 16);
			ptr += (dwBitLen / 16);

			// Convert exponent1 to 1
			for (n = 0; n < (dwBitLen / 16); n++)
			{
				if (n == 0) ptr[n] = 1;
				else ptr[n] = 0;
			}

			// Skip exponent1
			ptr += (dwBitLen / 16);

			// Convert exponent2 to 1
			for (n = 0; n < (dwBitLen / 16); n++)
			{
				if (n == 0) ptr[n] = 1;
				else ptr[n] = 0;
			}

			// Skip exponent2, coefficient
			ptr += (dwBitLen / 16);
			ptr += (dwBitLen / 16);

			// Convert privateExponent to 1
			for (n = 0; n < (dwBitLen / 8); n++)
			{
				if (n == 0) ptr[n] = 1;
				else ptr[n] = 0;
			}

			// Import the exponent-of-one private key.      
			if (!CryptImportKey(hProv, keyblob, dwkeyblob, 0, 0, hPrivateKey))
			{
				__leave;
			}

			fReturn = TRUE;
		}
		__finally
		{
			if (keyblob) LocalFree(keyblob);

			if (!fReturn)
			{
				if (*hPrivateKey) CryptDestroyKey(*hPrivateKey);
			}
		}

		return fReturn;
	}


	BOOL GenerateSessionKeyWithAlgorithm(HCRYPTPROV hProv,
		ALG_ID Alg,
		HCRYPTKEY *hSessionKey)
	{
		BOOL fResult;

		*hSessionKey = 0;

		fResult = CryptGenKey(hProv, Alg, CRYPT_EXPORTABLE, hSessionKey);
		if (!fResult)
		{
			return FALSE;
		}

		return TRUE;
	}


	BOOL DeriveSessionKeyWithAlgorithm(HCRYPTPROV hProv,
		ALG_ID Alg,
		LPBYTE lpHashingData,
		DWORD dwHashingData,
		HCRYPTKEY *hSessionKey)
	{
		BOOL fResult;
		BOOL fReturn = FALSE;
		HCRYPTHASH hHash = 0;

		__try
		{
			*hSessionKey = 0;

			fResult = CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash);
			if (!fResult) __leave;

			fResult = CryptHashData(hHash, lpHashingData, dwHashingData, 0);
			if (!fResult) __leave;

			fResult = CryptDeriveKey(hProv, Alg, hHash, CRYPT_EXPORTABLE, hSessionKey);
			if (!fResult) __leave;

			fReturn = TRUE;
		}
		__finally
		{
			if (hHash) CryptDestroyHash(hHash);
		}

		return fReturn;
	}


	BOOL ExportPlainSessionBlob(HCRYPTKEY hPublicKey,
		HCRYPTKEY hSessionKey,
		LPBYTE *pbKeyMaterial,
		DWORD *dwKeyMaterial)
	{
		BOOL fReturn = FALSE;
		BOOL fResult;
		DWORD dwSize, n;
		LPBYTE pbSessionBlob = NULL;
		DWORD dwSessionBlob;
		LPBYTE pbPtr;

		__try
		{
			*pbKeyMaterial = NULL;
			*dwKeyMaterial = 0;

			fResult = CryptExportKey(hSessionKey, hPublicKey, SIMPLEBLOB, 0, NULL, &dwSessionBlob);
			if (!fResult) __leave;

			pbSessionBlob = (LPBYTE)LocalAlloc(LPTR, dwSessionBlob);
			if (!pbSessionBlob) __leave;

			fResult = CryptExportKey(hSessionKey, hPublicKey, SIMPLEBLOB, 0, pbSessionBlob, &dwSessionBlob);
			if (!fResult) __leave;

			// Get session key size in bits
			dwSize = sizeof(DWORD);
			fResult = CryptGetKeyParam(hSessionKey, KP_KEYLEN, (LPBYTE)dwKeyMaterial, &dwSize, 0);
			if (!fResult) __leave;

			// Get the number of bytes and allocate buffer
			*dwKeyMaterial /= 8;
			*pbKeyMaterial = (LPBYTE)LocalAlloc(LPTR, *dwKeyMaterial);
			if (!*pbKeyMaterial) __leave;

			// Skip the header
			pbPtr = pbSessionBlob;
			pbPtr += sizeof(BLOBHEADER);
			pbPtr += sizeof(ALG_ID);

			// We are at the beginning of the key
			// but we need to start at the end since 
			// it's reversed
			pbPtr += (*dwKeyMaterial - 1);

			// Copy the raw key into our return buffer      
			for (n = 0; n < *dwKeyMaterial; n++)
			{
				(*pbKeyMaterial)[n] = *pbPtr;
				pbPtr--;
			}

			fReturn = TRUE;
		}
		__finally
		{
			if (pbSessionBlob) LocalFree(pbSessionBlob);

			if ((!fReturn) && (*pbKeyMaterial))
			{
				LocalFree(*pbKeyMaterial);
				*pbKeyMaterial = NULL;
				*dwKeyMaterial = 0;
			}
		}

		return fReturn;
	}


	BOOL ImportPlainSessionBlob(HCRYPTPROV hProv,
		HCRYPTKEY hPrivateKey,
		ALG_ID dwAlgId,
		LPBYTE pbKeyMaterial,
		DWORD dwKeyMaterial,
		HCRYPTKEY *hSessionKey)
	{
		BOOL fResult;
		BOOL fReturn = FALSE;
		BOOL fFound = FALSE;
		LPBYTE pbSessionBlob = NULL;
		DWORD dwSessionBlob, dwSize, n;
		DWORD dwPublicKeySize;
		DWORD dwProvSessionKeySize;
		ALG_ID dwPrivKeyAlg;
		LPBYTE pbPtr;
		DWORD dwFlags = CRYPT_FIRST;
		PROV_ENUMALGS_EX ProvEnum;
		HCRYPTKEY hTempKey = 0;

		__try
		{
			// Double check to see if this provider supports this algorithm
			// and key size
			do
			{
				dwSize = sizeof(ProvEnum);
				fResult = CryptGetProvParam(hProv, PP_ENUMALGS_EX, (LPBYTE)&ProvEnum, &dwSize, dwFlags);
				if (!fResult) break;

				dwFlags = 0;

				if (ProvEnum.aiAlgid == dwAlgId) fFound = TRUE;

			} while (!fFound);

			if (!fFound) __leave;

			// We have to get the key size(including padding)
			// from an HCRYPTKEY handle.  PP_ENUMALGS_EX contains
			// the key size without the padding so we can't use it.
			fResult = CryptGenKey(hProv, dwAlgId, 0, &hTempKey);
			if (!fResult) __leave;

			dwSize = sizeof(DWORD);
			fResult = CryptGetKeyParam(hTempKey, KP_KEYLEN, (LPBYTE)&dwProvSessionKeySize, &dwSize, 0);
			if (!fResult) __leave;
			CryptDestroyKey(hTempKey);
			hTempKey = 0;

			// Our key is too big, leave
			if ((dwKeyMaterial * 8) > dwProvSessionKeySize) __leave;

			// Get private key's algorithm
			dwSize = sizeof(ALG_ID);
			fResult = CryptGetKeyParam(hPrivateKey, KP_ALGID, (LPBYTE)&dwPrivKeyAlg, &dwSize, 0);
			if (!fResult) __leave;

			// Get private key's length in bits
			dwSize = sizeof(DWORD);
			fResult = CryptGetKeyParam(hPrivateKey, KP_KEYLEN, (LPBYTE)&dwPublicKeySize, &dwSize, 0);
			if (!fResult) __leave;

			// calculate Simple blob's length
			dwSessionBlob = (dwPublicKeySize / 8) + sizeof(ALG_ID) + sizeof(BLOBHEADER);

			// allocate simple blob buffer
			pbSessionBlob = (LPBYTE)LocalAlloc(LPTR, dwSessionBlob);
			if (!pbSessionBlob) __leave;

			pbPtr = pbSessionBlob;

			// SIMPLEBLOB Format is documented in SDK
			// Copy header to buffer
			((BLOBHEADER *)pbPtr)->bType = SIMPLEBLOB;
			((BLOBHEADER *)pbPtr)->bVersion = 2;
			((BLOBHEADER *)pbPtr)->reserved = 0;
			((BLOBHEADER *)pbPtr)->aiKeyAlg = dwAlgId;
			pbPtr += sizeof(BLOBHEADER);

			// Copy private key algorithm to buffer
			*((DWORD *)pbPtr) = dwPrivKeyAlg;
			pbPtr += sizeof(ALG_ID);

			// Place the key material in reverse order
			for (n = 0; n < dwKeyMaterial; n++)
			{
				pbPtr[n] = pbKeyMaterial[dwKeyMaterial - n - 1];
			}

			// 3 is for the first reserved byte after the key material + the 2 reserved bytes at the end.
			dwSize = dwSessionBlob - (sizeof(ALG_ID) + sizeof(BLOBHEADER) + dwKeyMaterial + 3);
			pbPtr += (dwKeyMaterial + 1);

			// Generate random data for the rest of the buffer
			// (except that last two bytes)
			fResult = CryptGenRandom(hProv, dwSize, pbPtr);
			if (!fResult) __leave;

			for (n = 0; n < dwSize; n++)
			{
				if (pbPtr[n] == 0) pbPtr[n] = 1;
			}

			pbSessionBlob[dwSessionBlob - 2] = 2;

			fResult = CryptImportKey(hProv, pbSessionBlob, dwSessionBlob,
				hPrivateKey, CRYPT_EXPORTABLE, hSessionKey);
			if (!fResult) __leave;

			fReturn = TRUE;
		}
		__finally
		{
			if (hTempKey) CryptDestroyKey(hTempKey);
			if (pbSessionBlob) LocalFree(pbSessionBlob);
		}

		return fReturn;
	}
}


namespace Poco {
namespace Crypto {


CipherKeyImpl::CipherKeyImpl(const std::string& name, 
	const std::string& passphrase, 
	const std::string& salt,
	int iterationCount):
	_hKey(0),
	_name(name),
	_id(id(name)),
	_key(keySize()),
	_iv(ivSize())
{
	generateKey(passphrase, salt, iterationCount);
	importKey();
	try
	{
		importIV();
	}
	catch (...)
	{
		CryptDestroyKey(_hKey);
		throw;
	}
}


CipherKeyImpl::CipherKeyImpl(const std::string& name, 
	const ByteVec& key, 
	const ByteVec& iv):
	_hKey(0),
	_name(name),
	_id(id(name)),
	_key(key),
	_iv(iv)
{
	if (_key.size() != keySize()) throw Poco::InvalidArgumentException("invalid key length");
	if (_iv.size() != ivSize()) throw Poco::InvalidArgumentException("invalid iv length");
	importKey();
	try
	{
		importIV();
	}
	catch (...)
	{
		CryptDestroyKey(_hKey);
		throw;
	}
}

	
CipherKeyImpl::CipherKeyImpl(const std::string& name):
	_hKey(0),
	_name(name),
	_id(id(name)),
	_key(keySize()),
	_iv(ivSize())
{
	generateKey();
	importKey();
	try
	{
		importIV();
	}
	catch (...)
	{
		CryptDestroyKey(_hKey);
		throw;
	}
}


CipherKeyImpl::~CipherKeyImpl()
{
	if (_hKey) CryptDestroyKey(_hKey);
}


CipherKeyImpl::Mode CipherKeyImpl::mode() const
{
	if (blockSize() != 0)
	{
		DWORD mode;
		DWORD modeLen = sizeof(mode);
		BOOL rc = CryptGetKeyParam(_hKey, KP_MODE, reinterpret_cast<BYTE*>(&mode), &modeLen, 0);
		if (rc)
		{
			switch (mode)
			{
			case CRYPT_MODE_ECB:
				return MODE_ECB;
			case CRYPT_MODE_CBC:
				return MODE_CBC;
			case CRYPT_MODE_OFB:
				return MODE_OFB;
			case CRYPT_MODE_CFB:
				return MODE_CFB;
			default:
				throw Poco::IllegalStateException("unexpected cipher mode");
			}
		}
		else throw Poco::SystemException("cannot get cipher mode");
	}
	else return MODE_STREAM_CIPHER;
}


void CipherKeyImpl::generateKey()
{
	getRandomBytes(_key, keySize());
	getRandomBytes(_iv, ivSize());
}


void CipherKeyImpl::getRandomBytes(ByteVec& vec, std::size_t count)
{
	Poco::RandomInputStream random;
	
	vec.clear();
	vec.reserve(count);

	for (int i = 0; i < count; ++i)
		vec.push_back(static_cast<unsigned char>(random.get()));
}


void CipherKeyImpl::generateKey(
	const std::string& password,
	const std::string& salt,
	int iterationCount)
{
	// We must be compatible with the OpenSSL implementation of Crypto.
	// OpenSSL documentation specifies that the salt must be an 8-byte array.
	unsigned char saltBytes[8];

	if (!salt.empty())
	{
		int len = static_cast<int>(salt.size());
		// Create the salt array from the salt string
		for (int i = 0; i < 8; ++i)
			saltBytes[i] = salt.at(i % len);
		for (int i = 8; i < len; ++i)
			saltBytes[i % 8] ^= salt.at(i);
	}

	// Now create the key and IV, using the MD5 digest algorithm.
	// We emulate the OpenSSL EVP_BytesToKey() function using MD5.
	
	// First iteration
	DigestEngine md5("MD5");
	md5.update(password.data(), password.size());
	if (!salt.empty()) md5.update(saltBytes, 8);
	Poco::DigestEngine::Digest d;
	d = md5.digest();
	for (int i = 1; i < iterationCount; i++)
	{
		md5.reset();
		md5.update(&d[0], d.size());
		d = md5.digest();
	}
	int keySz = keySize();
	int ivSz  = ivSize();
	int requiredSz = keySz + ivSz;
	int availableSz = static_cast<int>(d.size());
	int k = 1;
	Poco::DigestEngine::Digest extraD(d);
	while (availableSz < requiredSz)
	{
		md5.reset();
		md5.update(&extraD[0], extraD.size());
		md5.update(password.data(), password.size());
		if (!salt.empty()) md5.update(saltBytes, 8);
		extraD = md5.digest();
		for (int i = 1; i < iterationCount; i++)
		{
			md5.reset();
			md5.update(&extraD[0], extraD.size());
			extraD = md5.digest();
		}
		availableSz += static_cast<int>(extraD.size());
		d.insert(d.end(), extraD.begin(), extraD.end());
	}

	_key.assign(d.begin(), d.begin() + keySz);
	_iv.assign(d.begin() + keySz, d.begin() + requiredSz);
}


int CipherKeyImpl::keySize() const
{
	switch (_id)
	{
	case CALG_3DES: 
		return 192/8; // 168 key bits plus parity 
	case CALG_3DES_112:
		return 128/8; // 112 key bits plus parity
	case CALG_AES_128:
		return 128/8;
	case CALG_AES_192:
		return 192/8;
	case CALG_AES_256:
		return 256/8;
	case CALG_DES:
		return 64/8; // 56 key bits plus parity
	case CALG_RC2:
		return 128/8;
	case CALG_RC4:
		return 128/8;
	default:
		poco_bugcheck();
		return 0;
	}
}


int CipherKeyImpl::blockSize() const
{
	switch (_id)
	{
	case CALG_3DES: 
		return 64/8;
	case CALG_3DES_112:
		return 64/8;
	case CALG_AES_128:
	case CALG_AES_192:
	case CALG_AES_256:
		return 128/8;
	case CALG_DES:
		return 64/8;
	case CALG_RC2:
		return 64/8;
	case CALG_RC4:
		return 0;
	default:
		poco_bugcheck();
		return 0;
	}
}


int CipherKeyImpl::ivSize() const
{
	switch (_id)
	{
	case CALG_DES:
	case CALG_3DES: 
	case CALG_3DES_112:
		return 8;
	case CALG_AES_128:
	case CALG_AES_192:
	case CALG_AES_256:
		return 16;
	case CALG_RC2:
		return 8;
	case CALG_RC4:
		return 0;
	default:
		poco_bugcheck();
		return 0;
	}
}


void CipherKeyImpl::importKey()
{
	// Create Exponent of One private key
	HCRYPTKEY hPubPrivKey = 0;
	BOOL rc = CreatePrivateExponentOneKey(_sp.handle(), AT_KEYEXCHANGE, &hPubPrivKey);
	if (!rc) throw Poco::SystemException("cannot create private key for importing key", GetLastError());
	rc = ImportPlainSessionBlob(_sp.handle(), hPubPrivKey, _id, &_key[0], static_cast<DWORD>(_key.size()), &_hKey);
	CryptDestroyKey(hPubPrivKey);
	if (!rc) throw Poco::SystemException("cannot import key", GetLastError());
}


void CipherKeyImpl::importIV()
{
	if (!_iv.empty())
	{
		BOOL rc = CryptSetKeyParam(_hKey, KP_IV, static_cast<const BYTE*>(&_iv[0]), 0);
		if (!rc) throw Poco::SystemException("cannot import iv", GetLastError());
	}
}


ALG_ID CipherKeyImpl::id(const std::string& name)
{
	if (icompare(name, "3DES") == 0)
		return CALG_3DES;
	else if (icompare(name, "3DES-112") == 0 || icompare(name, "3DES112") == 0)
		return CALG_3DES_112;
	else if (icompare(name, "AES-128") == 0 || icompare(name, "AES128") == 0)
		return CALG_AES_128;
	else if (icompare(name, "AES-192") == 0 || icompare(name, "AES192") == 0)
		return CALG_AES_192;
	else if (icompare(name, "AES") == 0 || icompare(name, "AES-256") == 0 || icompare(name, "AES256") == 0)
		return CALG_AES_256;
	else if (icompare(name, "DES") == 0)
		return CALG_DES;
	else if (icompare(name, "RC2") == 0)
		return CALG_RC2;
	else if (icompare(name, "RC4") == 0)
		return CALG_RC4;
	else
		throw Poco::NotFoundException("cryptographic algorithm", name);
}


} } // namespace Poco::Crypto
