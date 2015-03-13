//
// RSACipherImpl.cpp
//
// $Id$
//
// Library: Crypto_Win
// Package: RSA
// Module:  RSACipherImpl
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/RSACipherImpl.h"
#include "Poco/Crypto/CryptoTransform.h"
#include "Poco/Error.h"
#include "Poco/Exception.h"


namespace Poco {
namespace Crypto {


namespace
{
	std::size_t rsaBlockSize(RSAKeyImpl::Ptr pKey)
	{
		DWORD blockLen = 0;
		DWORD dataLen = sizeof(DWORD);

		if (!CryptGetKeyParam(pKey->privateKey(), KP_BLOCKLEN, reinterpret_cast<LPBYTE>(&blockLen), &dataLen, 0))
		{
			error("Cannot obtain RSA block length");
		}
		poco_assert_dbg(dataLen = blockLen);

		return static_cast<std::size_t>(blockLen / 8);
	}


	class RSAEncryptImpl: public CryptoTransform
	{
	public:
		RSAEncryptImpl(const ServiceProvider& sp, RSAKeyImpl::Ptr pKey, RSAPaddingMode paddingMode);
		~RSAEncryptImpl();
		
		std::size_t blockSize() const;
		std::size_t maxDataSize() const;

		std::streamsize transform(
			const unsigned char* input,
			std::streamsize		 inputLength,
			unsigned char*		 output,
			std::streamsize		 outputLength);
		
		std::streamsize finalize(unsigned char*	output, std::streamsize length);

	private:
		DWORD encrypt(unsigned char* output, std::streamsize outputLength, BOOL isFinal);


		RSAKeyImpl::Ptr        _pKey;
		const ServiceProvider& _sp;
		RSAPaddingMode         _paddingMode;
		std::streamsize        _pos;
		unsigned char*         _pBuf;
	};


	RSAEncryptImpl::RSAEncryptImpl(const ServiceProvider& sp, RSAKeyImpl::Ptr pKey, RSAPaddingMode paddingMode) :
			_pKey(pKey),
			_sp(sp),
			_paddingMode(paddingMode),
			_pos(0),
			_pBuf(0)
	{
		_pBuf = new unsigned char[blockSize()];
	}


	RSAEncryptImpl::~RSAEncryptImpl()
	{
		delete [] _pBuf;
	}


	std::size_t RSAEncryptImpl::blockSize() const
	{
		return rsaBlockSize(_pKey);
	}


	std::size_t RSAEncryptImpl::maxDataSize() const
	{
		std::size_t size = blockSize();
		switch (_paddingMode)
		{
		case RSA_PADDING_PKCS1:
		case RSA_PADDING_SSLV23:
			size -= 11;
			break;
		case RSA_PADDING_PKCS1_OAEP:
			size -= 41;
			break;
		default:
			break;
		}
		return size;
	}

	DWORD RSAEncryptImpl::encrypt(unsigned char* output, std::streamsize outputLength, BOOL isFinal)
	{
		DWORD n = static_cast<DWORD>(_pos);
		if (!CryptEncrypt(_pKey->publicKey(), NULL, isFinal, 0, NULL, &n, 0))
			error("RSACipher cannot obtain length of encrypted data");
		poco_assert(n > _pos);
		std::vector<BYTE> data(n);
		n = static_cast<DWORD>(_pos /*+ 1*/);
		std::memcpy(&data[0], _pBuf, n);
		if (!CryptEncrypt(_pKey->publicKey(), NULL, isFinal, 0, &data[0], &n, static_cast<DWORD>(data.size())))
			error("RSACipher cannot encrypt data");
		poco_assert(n <= outputLength);
		std::memcpy(output, &data[0], n);
		return n;
	}

	std::streamsize RSAEncryptImpl::transform(const unsigned char* input,
		std::streamsize inputLength,
		unsigned char*  output,
		std::streamsize outputLength)
	{
		// always fill up the buffer before writing!
		std::streamsize maxSize = static_cast<std::streamsize>(maxDataSize());
		std::streamsize rsaSize = static_cast<std::streamsize>(blockSize());
		poco_assert_dbg(_pos <= maxSize);
		poco_assert (outputLength >= rsaSize);
		int rc = 0;
		while (inputLength > 0)
		{
			// check how many data bytes we are missing to get the buffer full
			poco_assert_dbg (maxSize >= _pos);
			std::streamsize missing = maxSize - _pos;
			if (missing == 0)
			{
				poco_assert (outputLength >= rsaSize);
				DWORD n = encrypt(output, outputLength, FALSE);
				rc += n;
				output += n;
				outputLength -= n;
				_pos = 0;
			}
			else
			{
				if (missing > inputLength)
					missing = inputLength;

				std::memcpy(_pBuf + _pos, input, static_cast<std::size_t>(missing));
				input += missing;
				_pos += missing;
				inputLength -= missing;
			}
		}
		return rc;
	}


	std::streamsize RSAEncryptImpl::finalize(unsigned char*	output, std::streamsize length)
	{
		poco_assert (length >= blockSize());
		poco_assert (_pos <= maxDataSize());
		int rc = 0;
		if (_pos > 0)
		{
			rc = encrypt(output, length, TRUE);
		}
		return rc;
	}


	class RSADecryptImpl: public CryptoTransform
	{
	public:
		RSADecryptImpl(const ServiceProvider& sp, RSAKeyImpl::Ptr pKey, RSAPaddingMode paddingMode);
		~RSADecryptImpl();
		
		std::size_t blockSize() const;

		std::streamsize transform(const unsigned char* input,
			std::streamsize		 inputLength,
			unsigned char*		 output,
			std::streamsize		 outputLength);
		
		std::streamsize finalize(
			unsigned char*	output,
			std::streamsize length);

	private:
		DWORD decrypt(unsigned char* output, std::streamsize outputLength, BOOL isFinal);

		const ServiceProvider& _sp;
		RSAKeyImpl::Ptr        _pKey;
		RSAPaddingMode         _paddingMode;
		std::streamsize        _pos;
		unsigned char*         _pBuf;
	};


	RSADecryptImpl::RSADecryptImpl(const ServiceProvider& sp, RSAKeyImpl::Ptr pKey, RSAPaddingMode paddingMode) :
			_pKey(pKey),
			_sp(sp),
			_paddingMode(paddingMode),
			_pos(0),
			_pBuf(0)
	{
		_pBuf = new unsigned char[blockSize()];
	}


	RSADecryptImpl::~RSADecryptImpl()
	{
		delete [] _pBuf;
	}


	std::size_t RSADecryptImpl::blockSize() const
	{
		return rsaBlockSize(_pKey);
	}


	DWORD RSADecryptImpl::decrypt(unsigned char* output, std::streamsize outputLength, BOOL isFinal)
	{
		DWORD n = static_cast<DWORD>(_pos);
		std::vector<BYTE> data(n);
		std::memcpy(&data[0], _pBuf, n);
		if (!CryptDecrypt(_pKey->privateKey(), NULL, isFinal, 0, &data[0], &n))
			error("RSACipher cannot decrypt data");
		poco_assert(n <= outputLength);
		std::memcpy(output, &data[0], n);
		return n;
	}


	std::streamsize RSADecryptImpl::transform(const unsigned char* input,
		std::streamsize		 inputLength,
		unsigned char*		 output,
		std::streamsize		 outputLength)
	{
		
		// always fill up the buffer before decrypting!
		std::streamsize rsaSize = static_cast<std::streamsize>(blockSize());
		poco_assert_dbg(_pos <= rsaSize);
		poco_assert (outputLength >= rsaSize);
		int rc = 0;
		while (inputLength > 0)
		{
			// check how many data bytes we are missing to get the buffer full
			poco_assert_dbg (rsaSize >= _pos);
			std::streamsize missing = rsaSize - _pos;
			if (missing == 0)
			{
				DWORD tmp = decrypt(output, outputLength, FALSE);
				if (tmp == -1) error("RSACipher cannot transform data");
				rc += tmp;
				output += tmp;
				outputLength -= tmp;
				_pos = 0;
				
			}
			else
			{
				if (missing > inputLength)
					missing = inputLength;

				std::memcpy(_pBuf + _pos, input, static_cast<std::size_t>(missing));
				input += missing;
				_pos += missing;
				inputLength -= missing;
			}
		}
		return rc;
	}


	std::streamsize RSADecryptImpl::finalize(unsigned char*	output, std::streamsize length)
	{
		poco_assert (length >= blockSize());
		int rc = 0;
		if (_pos > 0)
		{
			rc = decrypt(output, length, TRUE);
			if (rc == -1) error("RSACipher cannot transform data");
		}
		return rc;
	}
}


RSACipherImpl::RSACipherImpl(const RSAKey& key, RSAPaddingMode paddingMode):
	_key(key),
	_paddingMode(paddingMode)
{
}


RSACipherImpl::~RSACipherImpl()
{
}


CryptoTransform* RSACipherImpl::createEncryptor()
{
	return new RSAEncryptImpl(_key.impl()->serviceProvider(), _key.impl(), _paddingMode);
}


CryptoTransform* RSACipherImpl::createDecryptor()
{
	return new RSADecryptImpl(_key.impl()->serviceProvider(), _key.impl(), _paddingMode);
}


} } // namespace Poco::Crypto
