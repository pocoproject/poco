//
// EVPCipherImpl.cpp
//
// Library: Crypto
// Package: EVP
// Module:  EVPCipherImpl
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/EVPCipherImpl.h"
#include "Poco/Crypto/CryptoTransform.h"
#include "Poco/Exception.h"
#include "Poco/Logger.h"
#include <openssl/err.h>
#include <openssl/evp.h>
#include <cstring>


namespace Poco {
namespace Crypto {


namespace
{
	void throwError(std::string&& msg)
	{
		unsigned long err;
		while ((err = ERR_get_error()))
		{
			if (!msg.empty()) msg.append("; ");
			msg.append(ERR_error_string(err, 0));
		}

		throw Poco::IOException(msg);
	}

	class EVPPKeyContext
	{
	public:
		EVPPKeyContext() = delete;
		EVPPKeyContext(const EVP_PKEY* pEVP) : _pCtx(EVP_PKEY_CTX_new(const_cast<EVP_PKEY*>(pEVP), NULL))
		{
			if (!_pCtx)
			{
				std::string fmt = "EVPPKeyContext():%s()";
				throwError(Poco::format(fmt, std::string("EVP_PKEY_CTX_new")));
			}
		}

		~EVPPKeyContext()
		{
			EVP_PKEY_CTX_free(_pCtx);
		}

		operator EVP_PKEY_CTX*()
		{
			return _pCtx;
		}

	private:
		EVP_PKEY_CTX* _pCtx = nullptr;
	};

	class EVPEncryptImpl: public CryptoTransform
	{
	public:
		EVPEncryptImpl(const EVP_PKEY* pEVP):
			_pEVP(pEVP),
			_pCtx(_pEVP),
			_pos(0),
			_pBuf(0)
		{
			std::string fmt = "EVPEncryptImpl():%s()";
			poco_check_ptr(_pEVP);

			if (!_pCtx)
				throwError(Poco::format(fmt, std::string("EVP_PKEY_CTX_new")));

			if (EVP_PKEY_encrypt_init(_pCtx) <= 0)
        		throwError(Poco::format(fmt, std::string("EVP_PKEY_encrypt_init")));

			_blockSize = EVP_PKEY_size(const_cast<EVP_PKEY*>(_pEVP));
			if (!_blockSize)
				throwError(Poco::format(fmt, std::string("EVP_PKEY_size")));
			_pBuf = new unsigned char[_blockSize];
		}

		~EVPEncryptImpl()
		{
			delete [] _pBuf;
		}

		std::size_t blockSize() const
		{
			return _blockSize;
		}

		std::string getTag(std::size_t)
		{
			return "";
		}

		void setTag(const std::string&)
		{
		}

		std::streamsize transform(const unsigned char* input, std::streamsize inputLength,
			unsigned char* output, std::streamsize outputLength)
		{
			std::string fmt = "EVPEncryptImpl::transform():%s()";
			std::streamsize maxSize = static_cast<std::streamsize>(maxDataSize(input, inputLength));
			std::streamsize evpSize = static_cast<std::streamsize>(_blockSize);
			poco_assert_dbg(_pos <= maxSize);
			poco_assert (outputLength >= evpSize);
			int rc = 0;
			while (inputLength > 0)
			{
				poco_assert_dbg (maxSize >= _pos);
				std::streamsize missing = maxSize - _pos;
				if (missing == 0)
				{
					poco_assert (outputLength >= evpSize);
					std::size_t outLen;
					if (EVP_PKEY_encrypt(_pCtx, NULL, &outLen, _pBuf, static_cast<std::size_t>(maxSize)) <= 0)
						throwError(Poco::format(fmt, std::string("EVP_PKEY_encrypt(NULL)")));
					if (EVP_PKEY_encrypt(_pCtx, output, &outLen, _pBuf, static_cast<std::size_t>(maxSize)) <= 0)
						throwError(Poco::format(fmt, std::string("EVP_PKEY_encrypt")));
					rc += static_cast<int>(outLen);
					output += outLen;
					outputLength -= outLen;
					_pos = 0;

				}
				else
				{
					if (missing > inputLength) missing = inputLength;
					std::memcpy(_pBuf + _pos, input, static_cast<std::size_t>(missing));
					input += missing;
					_pos += missing;
					inputLength -= missing;
				}
			}
			return rc;
		}

		std::streamsize finalize(unsigned char*	output, std::streamsize length)
		{
			poco_assert (length >= blockSize());
			poco_assert (_pos <= maxDataSize(output, length));
			std::string fmt = "EVPEncryptImpl::finalize():%s()";
			std::size_t outLen = 0;
			if (_pos > 0)
			{
				if (EVP_PKEY_encrypt(_pCtx, NULL, &outLen, _pBuf, static_cast<std::size_t>(_pos)) <= 0)
					throwError(Poco::format(fmt, std::string("EVP_PKEY_encrypt")));
				if (EVP_PKEY_encrypt(_pCtx, output, &outLen, _pBuf, static_cast<std::size_t>(_pos)) <= 0)
					throwError(Poco::format(fmt, std::string("EVP_PKEY_encrypt")));
			}
			return static_cast<std::streamsize>(outLen);
		}

	private:
		std::size_t maxDataSize(const unsigned char* pIO, std::streamsize length)
		{
			std::string fmt = "EVPEncryptImpl::maxDataSize():%s()";
			std::size_t outLength = 0;
			if (EVP_PKEY_encrypt(_pCtx, NULL, &outLength, pIO, length) <= 0)
				throwError(Poco::format(fmt, std::string("EVP_PKEY_encrypt")));
			return outLength;
		}

		const EVP_PKEY* _pEVP;
		EVPPKeyContext  _pCtx;
		int             _blockSize;
		std::streamsize _pos;
		unsigned char*  _pBuf;
	};


	class EVPDecryptImpl: public CryptoTransform
	{
	public:
		EVPDecryptImpl(const EVP_PKEY* pEVP):
			_pEVP(pEVP),
			_pCtx(_pEVP),
			_pos(0),
			_pBuf(0)
		{
			std::string fmt = "EVPDecryptImpl():%s()";
			poco_check_ptr(_pEVP);

			if (EVP_PKEY_decrypt_init(_pCtx) <= 0)
        		throwError(Poco::format(fmt, std::string("EVP_PKEY_decrypt_init")));

			_blockSize = EVP_PKEY_size(const_cast<EVP_PKEY*>(_pEVP));
			if (!_blockSize)
				throwError(Poco::format(fmt, std::string("EVP_PKEY_size")));
			_pBuf = new unsigned char[_blockSize];
		}

		~EVPDecryptImpl()
		{
			delete [] _pBuf;
		}

		std::size_t blockSize() const
		{
			return _blockSize;
		}

		std::string getTag(std::size_t)
		{
			return "";
		}

		void setTag(const std::string&)
		{
		}

		std::streamsize transform(const unsigned char* input, std::streamsize inputLength,
			unsigned char* output, std::streamsize outputLength)
		{
			std::string fmt = "EVPDecryptImpl::transform():%s()";
			std::streamsize evpSize = static_cast<std::streamsize>(_blockSize);
			poco_assert_dbg(_pos <= evpSize);
			poco_assert (outputLength >= evpSize);
			int rc = 0;
			while (inputLength > 0)
			{
				poco_assert_dbg (evpSize >= _pos);
				std::streamsize missing = evpSize - _pos;
				if (missing == 0)
				{
					std::size_t outLen = 0;
					if (EVP_PKEY_decrypt(_pCtx, NULL, &outLen, _pBuf, static_cast<std::size_t>(_pos)) <= 0)
						throwError(Poco::format(fmt, std::string("EVP_PKEY_decrypt(NULL)")));
					if (EVP_PKEY_decrypt(_pCtx, output, &outLen, _pBuf, static_cast<std::size_t>(_pos)) <= 0)
						throwError(Poco::format(fmt, std::string("EVP_PKEY_decrypt")));
					rc += static_cast<int>(outLen);
					output += outLen;
					outputLength -= outLen;
					_pos = 0;
				}
				else
				{
					if (missing > inputLength) missing = inputLength;
					std::memcpy(_pBuf + _pos, input, static_cast<std::size_t>(missing));
					input += missing;
					_pos += missing;
					inputLength -= missing;
				}
			}
			return rc;
		}

		std::streamsize finalize(unsigned char* output, std::streamsize length)
		{
			poco_assert (length >= _blockSize);
			std::string fmt = "EVPDecryptImpl::finalize():%s()";
			std::size_t outLen = 0;
			if (EVP_PKEY_decrypt(_pCtx, NULL, &outLen, _pBuf, static_cast<std::size_t>(_pos)) <= 0)
					throwError(Poco::format(fmt, std::string("EVP_PKEY_decrypt(NULL)")));
			poco_assert (length >= outLen);
			if (_pos > 0)
			{
				if (EVP_PKEY_decrypt(_pCtx, output, &outLen, _pBuf, static_cast<std::size_t>(_pos)) <= 0)
					throwError(Poco::format(fmt, std::string("EVP_PKEY_decrypt")));
			}
			return outLen;
		}

	private:
		const EVP_PKEY* _pEVP;
		EVPPKeyContext  _pCtx;
		int             _blockSize;
		std::streamsize _pos;
		unsigned char*  _pBuf;
	};
}


EVPCipherImpl::EVPCipherImpl(const EVPPKey& key):
	_key(key)
{
}


EVPCipherImpl::~EVPCipherImpl()
{
}


CryptoTransform::Ptr EVPCipherImpl::createEncryptor()
{
	return new EVPEncryptImpl(_key);
}


CryptoTransform::Ptr EVPCipherImpl::createDecryptor()
{
	return new EVPDecryptImpl(_key);
}


} } // namespace Poco::Crypto
