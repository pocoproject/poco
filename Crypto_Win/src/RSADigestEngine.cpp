//
// RSADigestEngine.cpp
//
// $Id$
//
// Library: Crypto_Win
// Package: RSA
// Module:  RSADigestEngine
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/RSADigestEngine.h"
#include "Poco/Format.h"
#include "Poco/Error.h"
#include "Poco/Base64Encoder.h"
#include <sstream>


namespace Poco {
namespace Crypto {


RSADigestEngine::RSADigestEngine(const RSAKey& key, DigestType digestType):
	_key(key),
	_engine(digestType == DIGEST_MD5 ? static_cast<Poco::DigestEngine&>(_md5Engine) : static_cast<Poco::DigestEngine&>(_sha1Engine)),
	_type(digestType == DIGEST_MD5 ? szOID_RSA_MD5 : szOID_ECDSA_SHA1)
{
}


RSADigestEngine::~RSADigestEngine()
{
}


std::size_t RSADigestEngine::digestLength() const
{
	return _engine.digestLength();
}


void RSADigestEngine::reset()
{
	_engine.reset();
	_digest.clear();
	_signature.clear();
}

	
const DigestEngine::Digest& RSADigestEngine::digest()
{
	if (_digest.empty())
	{
		_digest = _engine.digest();
	}
	return _digest;
}


const DigestEngine::Digest& RSADigestEngine::signature()
{
	if (_signature.empty())
	{
		digest();

		std::string begin = "-----BEGIN RSA PUBLIC KEY-----\n";
		//TODO: base64 encode data and pass to CryptImportKey API
		std::string sig(_digest.begin(), _digest.end());
		std::string end = "\n-----END RSA PUBLIC KEY-----";

		std::ostringstream ostr;
		Base64Encoder encoder(ostr);
		encoder << sig;
		encoder.close();

		begin.append(ostr.str()).append(end);
		_signature.assign(begin.begin(), begin.end());
	}

	return _signature;
}

	
bool RSADigestEngine::verify(const DigestEngine::Digest& sig)
{
	digest();

	DWORD cbData = 0;
	BYTE* pbData = NULL;

	if (CryptExportKey(_key.impl()->privateKey(), 0, PRIVATEKEYBLOB, 0/*???*/,
		NULL, &cbData))
	{
		std::vector<BYTE> pkData(cbData);
		pbData = &pkData[0];

		if (CryptExportKey(_key.impl()->publicKey(), 0, PRIVATEKEYBLOB, 0/*???*/,
			pbData, &cbData))
		{
			// TODO: base64-decode pbData
		}
		else // !CryptExportKey
		{
			DWORD err = Error::last();
			std::string errStr = Error::getMessage(err);
			switch (err)
			{
			case ERROR_INVALID_HANDLE: case ERROR_INVALID_PARAMETER:
			case NTE_BAD_DATA:         case NTE_BAD_FLAGS:
			case NTE_BAD_KEY:          case NTE_BAD_KEY_STATE:
			case NTE_BAD_PUBLIC_KEY:   case NTE_BAD_TYPE:
			case NTE_BAD_UID:          case NTE_NO_KEY:
				throw Poco::InvalidArgumentException(errStr);
			default:
				throw  Poco::SystemException("Cannot export public key.");
			}
		}
	}
	else // !CryptExportKey length
	{
		throw  Poco::SystemException("Cannot obtain key export length.");
	}

	return false;
}


void RSADigestEngine::updateImpl(const void* data, std::size_t length)
{
	_engine.update(data, length);
}


} } // namespace Poco::Crypto
