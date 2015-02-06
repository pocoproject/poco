//
// CipherImpl.cpp
//
// $Id$
//
// Library: Crypto_Win
// Package: Cipher
// Module:  CipherImpl
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/CipherImpl.h"
#include "Poco/Crypto/CryptoTransform.h"
#include "Poco/Exception.h"
#include "Poco/Buffer.h"
#include <WinCrypt.h>


namespace Poco {
namespace Crypto {


class CryptoTransformImpl: public CryptoTransform
{
public:
	enum Direction
	{
		DIR_ENCRYPT,
		DIR_DECRYPT
	};

	CryptoTransformImpl(CipherKeyImpl::Ptr pKey, Direction dir);
	~CryptoTransformImpl();
	std::size_t blockSize() const;
	int setPadding(int padding);	

	std::streamsize transform(
		const unsigned char* input,
		std::streamsize      inputLength,
		unsigned char*       output,
		std::streamsize      outputLength);
		
	std::streamsize finalize(
		unsigned char*  output,
		std::streamsize length);

private:
	CipherKeyImpl::Ptr _pKey;
	Direction _dir;
	Poco::Buffer<char> _buffer;
	std::size_t _leftOver;
};


CryptoTransformImpl::CryptoTransformImpl(CipherKeyImpl::Ptr pKey, Direction dir):
	_pKey(pKey),
	_dir(dir),
	_buffer(2*_pKey->blockSize()),
	_leftOver(0)
{
}


CryptoTransformImpl::~CryptoTransformImpl()
{
}


std::size_t CryptoTransformImpl::blockSize() const
{
	return _pKey->blockSize();
}

	
int CryptoTransformImpl::setPadding(int padding)
{
	if (padding != 1) throw Poco::NotImplementedException("padding cannot be disabled");
	return 1;
}
	

std::streamsize CryptoTransformImpl::transform(
	const unsigned char* input,
	std::streamsize      inputLength,
	unsigned char*       output,
	std::streamsize      outputLength)
{
	// CryptEncrypt and CryptDecrypt require a multiple of block size
	std::size_t blockSz = blockSize();
	poco_assert (outputLength >= (inputLength + blockSz));

	int blocks = static_cast<int>((_leftOver + inputLength)/blockSz);
	if (blocks > 0)
	{
		std::streamsize blockedSize = blocks*blockSz;
		std::streamsize processed = blockedSize - _leftOver;

		poco_assert (blockedSize <= outputLength);

		std::memcpy(output, _buffer.begin(), _leftOver);
		std::memcpy(output + _leftOver, input, static_cast<std::size_t>(processed));
	
		_leftOver = static_cast<std::size_t>(inputLength - processed);
		if (_leftOver == 0)
		{
			// always leave something over for finalize
			_leftOver = blockSz;
			if (processed > blockSz)
				processed -= blockSz;
			else
				processed = 0;
			blockedSize -= blockSz;
		}
		std::memcpy(_buffer.begin(), input + processed, _leftOver);

		DWORD dataLen = static_cast<DWORD>(blockedSize);
		DWORD bufLen = static_cast<DWORD>(outputLength);
		if (dataLen > 0)
		{
			if (_dir == DIR_ENCRYPT)
			{
				BOOL rc = CryptEncrypt(_pKey->_hKey, 0, FALSE, 0, reinterpret_cast<BYTE*>(output), &dataLen, bufLen);
				DWORD err = GetLastError();
				if (!rc) throw Poco::IOException("failed to encrypt data", GetLastError());
			}
			else
			{
				BOOL rc = CryptDecrypt(_pKey->_hKey, 0, FALSE, 0, reinterpret_cast<BYTE*>(output), &dataLen);
				if (!rc) throw Poco::IOException("failed to decrypt data", GetLastError());
			}
		}
		return static_cast<std::streamsize>(dataLen);
	}
	else
	{
		poco_assert_dbg (_leftOver + inputLength < blockSz);
		std::memcpy(_buffer.begin() + _leftOver, input, static_cast<std::size_t>(inputLength));
		_leftOver += static_cast<std::size_t>(inputLength);
		return 0;
	}
}


std::streamsize CryptoTransformImpl::finalize(
	unsigned char*	output,
	std::streamsize length)
{
	poco_assert (length >= blockSize());
		
	std::memcpy(output, _buffer.begin(), _leftOver);
	DWORD dataLen = static_cast<DWORD>(_leftOver);
	DWORD bufLen = static_cast<DWORD>(length);
	if (_dir == DIR_ENCRYPT)
	{
		BOOL rc = CryptEncrypt(_pKey->_hKey, 0, TRUE, 0, reinterpret_cast<BYTE*>(output), &dataLen, bufLen);
		if (!rc) throw Poco::IOException("failed to encrypt data", GetLastError());
	}
	else 
	{
		BOOL rc = CryptDecrypt(_pKey->_hKey, 0, TRUE, 0, reinterpret_cast<BYTE*>(output), &dataLen);
		if (!rc) throw Poco::IOException("failed to decrypt data", GetLastError());
	}

	return static_cast<std::streamsize>(dataLen);
}


CipherImpl::CipherImpl(const CipherKey& key):
	_key(key)
{
}


CipherImpl::~CipherImpl()
{
}


CryptoTransform* CipherImpl::createEncryptor()
{
	CipherKeyImpl::Ptr pKey = _key.impl();
	return new CryptoTransformImpl(pKey, CryptoTransformImpl::DIR_ENCRYPT);
}


CryptoTransform* CipherImpl::createDecryptor()
{
	CipherKeyImpl::Ptr pKey = _key.impl();
	return new CryptoTransformImpl(pKey, CryptoTransformImpl::DIR_DECRYPT);
}


} } // namespace Poco::Crypto
