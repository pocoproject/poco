//
// RSACipherImpl.cpp
//
// Library: Crypto
// Package: RSA
// Module:  RSACipherImpl
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/RSACipherImpl.h"
#include "Poco/Crypto/CryptoTransform.h"
#include "Poco/Exception.h"
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <cstring>


namespace Poco::Crypto {


namespace
{
	void throwError()
	{
		unsigned long err;
		std::string msg;

		while ((err = ERR_get_error()))
		{
			if (!msg.empty())
				msg.append("; ");
			msg.append(ERR_error_string(err, nullptr));
		}

		throw Poco::IOException(msg);
	}


	int mapPaddingMode(RSAPaddingMode paddingMode)
	{
		switch (paddingMode)
		{
		case RSA_PADDING_PKCS1:
			return RSA_PKCS1_PADDING;
		case RSA_PADDING_PKCS1_OAEP:
			return RSA_PKCS1_OAEP_PADDING;
		case RSA_PADDING_NONE:
			return RSA_NO_PADDING;
		default:
			poco_bugcheck();
			return RSA_NO_PADDING;
		}
	}


#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)


	class RSAEncryptImpl: public CryptoTransform
	{
	public:
		RSAEncryptImpl(EVP_PKEY* pKey, RSAPaddingMode paddingMode);
		~RSAEncryptImpl();

		std::size_t blockSize() const;
		std::size_t maxDataSize() const;
		std::string getTag(std::size_t);
		void setTag(const std::string&);

		std::streamsize transform(
			const unsigned char* input,
			std::streamsize		 inputLength,
			unsigned char*		 output,
			std::streamsize		 outputLength);

		std::streamsize finalize(unsigned char* output, std::streamsize length);

	private:
		EVP_PKEY*       _pKey;
		RSAPaddingMode  _paddingMode;
		std::streamsize _pos;
		unsigned char*  _pBuf;
	};


	RSAEncryptImpl::RSAEncryptImpl(EVP_PKEY* pKey, RSAPaddingMode paddingMode):
			_pKey(pKey),
			_paddingMode(paddingMode),
			_pos(0),
			_pBuf(nullptr)
	{
		_pBuf = new unsigned char[blockSize()];
	}


	RSAEncryptImpl::~RSAEncryptImpl()
	{
		delete [] _pBuf;
	}


	std::size_t RSAEncryptImpl::blockSize() const
	{
		return EVP_PKEY_get_size(_pKey);
	}


	std::size_t RSAEncryptImpl::maxDataSize() const
	{
		std::size_t size = blockSize();
		switch (_paddingMode)
		{
		case RSA_PADDING_PKCS1:
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


	std::string RSAEncryptImpl::getTag(std::size_t)
	{
		return std::string();
	}


	void RSAEncryptImpl::setTag(const std::string&)
	{
	}


	std::streamsize RSAEncryptImpl::transform(
		const unsigned char* input,
		std::streamsize		 inputLength,
		unsigned char*		 output,
		std::streamsize		 outputLength)
	{
		std::streamsize maxSize = static_cast<std::streamsize>(maxDataSize());
		std::streamsize rsaSize = static_cast<std::streamsize>(blockSize());
		poco_assert_dbg(_pos <= maxSize);
		poco_assert (outputLength >= rsaSize);
		int rc = 0;
		while (inputLength > 0)
		{
			poco_assert_dbg (maxSize >= _pos);
			std::streamsize missing = maxSize - _pos;
			if (missing == 0)
			{
				poco_assert (outputLength >= rsaSize);
				EVP_PKEY_CTX* pCtx = EVP_PKEY_CTX_new(_pKey, nullptr);
				if (!pCtx) throwError();
				if (EVP_PKEY_encrypt_init(pCtx) != 1) { EVP_PKEY_CTX_free(pCtx); throwError(); }
				if (EVP_PKEY_CTX_set_rsa_padding(pCtx, mapPaddingMode(_paddingMode)) != 1) { EVP_PKEY_CTX_free(pCtx); throwError(); }
				size_t outLen = static_cast<size_t>(rsaSize);
				if (EVP_PKEY_encrypt(pCtx, output, &outLen, _pBuf, static_cast<size_t>(maxSize)) != 1)
				{
					EVP_PKEY_CTX_free(pCtx);
					throwError();
				}
				EVP_PKEY_CTX_free(pCtx);
				rc += static_cast<int>(outLen);
				output += outLen;
				outputLength -= outLen;
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


	std::streamsize RSAEncryptImpl::finalize(unsigned char* output, std::streamsize length)
	{
		poco_assert (length >= static_cast<std::streamsize>(blockSize()));
		poco_assert (static_cast<std::size_t>(_pos) <= maxDataSize());
		int rc = 0;
		if (_pos > 0)
		{
			EVP_PKEY_CTX* pCtx = EVP_PKEY_CTX_new(_pKey, nullptr);
			if (!pCtx) throwError();
			if (EVP_PKEY_encrypt_init(pCtx) != 1) { EVP_PKEY_CTX_free(pCtx); throwError(); }
			if (EVP_PKEY_CTX_set_rsa_padding(pCtx, mapPaddingMode(_paddingMode)) != 1) { EVP_PKEY_CTX_free(pCtx); throwError(); }
			size_t outLen = static_cast<size_t>(length);
			if (EVP_PKEY_encrypt(pCtx, output, &outLen, _pBuf, static_cast<size_t>(_pos)) != 1)
			{
				EVP_PKEY_CTX_free(pCtx);
				throwError();
			}
			EVP_PKEY_CTX_free(pCtx);
			rc = static_cast<int>(outLen);
		}
		return rc;
	}


	class RSADecryptImpl: public CryptoTransform
	{
	public:
		RSADecryptImpl(EVP_PKEY* pKey, RSAPaddingMode paddingMode);
		~RSADecryptImpl();

		std::size_t blockSize() const;
		std::string getTag(std::size_t);
		void setTag(const std::string&);

		std::streamsize transform(
			const unsigned char* input,
			std::streamsize		 inputLength,
			unsigned char*		 output,
			std::streamsize		 outputLength);

		std::streamsize finalize(
			unsigned char*	output,
			std::streamsize length);

	private:
		EVP_PKEY*       _pKey;
		RSAPaddingMode  _paddingMode;
		std::streamsize _pos;
		unsigned char*  _pBuf;
	};


	RSADecryptImpl::RSADecryptImpl(EVP_PKEY* pKey, RSAPaddingMode paddingMode):
			_pKey(pKey),
			_paddingMode(paddingMode),
			_pos(0),
			_pBuf(nullptr)
	{
		_pBuf = new unsigned char[blockSize()];
	}


	RSADecryptImpl::~RSADecryptImpl()
	{
		delete [] _pBuf;
	}


	std::size_t RSADecryptImpl::blockSize() const
	{
		return EVP_PKEY_get_size(_pKey);
	}


	std::string RSADecryptImpl::getTag(std::size_t)
	{
		return std::string();
	}


	void RSADecryptImpl::setTag(const std::string&)
	{
	}


	std::streamsize RSADecryptImpl::transform(
		const unsigned char* input,
		std::streamsize		 inputLength,
		unsigned char*		 output,
		std::streamsize		 outputLength)
	{
		std::streamsize rsaSize = static_cast<std::streamsize>(blockSize());
		poco_assert_dbg(_pos <= rsaSize);
		poco_assert (outputLength >= rsaSize);
		int rc = 0;
		while (inputLength > 0)
		{
			poco_assert_dbg (rsaSize >= _pos);
			std::streamsize missing = rsaSize - _pos;
			if (missing == 0)
			{
				EVP_PKEY_CTX* pCtx = EVP_PKEY_CTX_new(_pKey, nullptr);
				if (!pCtx) throwError();
				if (EVP_PKEY_decrypt_init(pCtx) != 1) { EVP_PKEY_CTX_free(pCtx); throwError(); }
				if (EVP_PKEY_CTX_set_rsa_padding(pCtx, mapPaddingMode(_paddingMode)) != 1) { EVP_PKEY_CTX_free(pCtx); throwError(); }
				size_t outLen = static_cast<size_t>(rsaSize);
				int tmp = EVP_PKEY_decrypt(pCtx, output, &outLen, _pBuf, static_cast<size_t>(rsaSize));
				EVP_PKEY_CTX_free(pCtx);
				if (tmp != 1) throwError();
				rc += static_cast<int>(outLen);
				output += outLen;
				outputLength -= outLen;
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


	std::streamsize RSADecryptImpl::finalize(unsigned char* output, std::streamsize length)
	{
		poco_assert (length >= static_cast<std::streamsize>(blockSize()));
		int rc = 0;
		if (_pos > 0)
		{
			EVP_PKEY_CTX* pCtx = EVP_PKEY_CTX_new(_pKey, nullptr);
			if (!pCtx) throwError();
			if (EVP_PKEY_decrypt_init(pCtx) != 1) { EVP_PKEY_CTX_free(pCtx); throwError(); }
			if (EVP_PKEY_CTX_set_rsa_padding(pCtx, mapPaddingMode(_paddingMode)) != 1) { EVP_PKEY_CTX_free(pCtx); throwError(); }
			size_t outLen = static_cast<size_t>(length);
			int tmp = EVP_PKEY_decrypt(pCtx, output, &outLen, _pBuf, static_cast<size_t>(_pos));
			EVP_PKEY_CTX_free(pCtx);
			if (tmp != 1) throwError();
			rc = static_cast<int>(outLen);
		}
		return rc;
	}


#else // !POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)


	class RSAEncryptImpl: public CryptoTransform
	{
	public:
		RSAEncryptImpl(const RSA* pRSA, RSAPaddingMode paddingMode);
		~RSAEncryptImpl();

		std::size_t blockSize() const;
		std::size_t maxDataSize() const;
		std::string getTag(std::size_t);
		void setTag(const std::string&);

		std::streamsize transform(
			const unsigned char* input,
			std::streamsize		 inputLength,
			unsigned char*		 output,
			std::streamsize		 outputLength);

		std::streamsize finalize(unsigned char* output, std::streamsize length);

	private:
		const RSA*      _pRSA;
		RSAPaddingMode  _paddingMode;
		std::streamsize _pos;
		unsigned char*  _pBuf;
	};


	RSAEncryptImpl::RSAEncryptImpl(const RSA* pRSA, RSAPaddingMode paddingMode):
			_pRSA(pRSA),
			_paddingMode(paddingMode),
			_pos(0),
			_pBuf(nullptr)
	{
		_pBuf = new unsigned char[blockSize()];
	}


	RSAEncryptImpl::~RSAEncryptImpl()
	{
		delete [] _pBuf;
	}


	std::size_t RSAEncryptImpl::blockSize() const
	{
		return RSA_size(_pRSA);
	}


	std::size_t RSAEncryptImpl::maxDataSize() const
	{
		std::size_t size = blockSize();
		switch (_paddingMode)
		{
		case RSA_PADDING_PKCS1:
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


	std::string RSAEncryptImpl::getTag(std::size_t)
	{
		return std::string();
	}


	void RSAEncryptImpl::setTag(const std::string&)
	{
	}


	std::streamsize RSAEncryptImpl::transform(
		const unsigned char* input,
		std::streamsize		 inputLength,
		unsigned char*		 output,
		std::streamsize		 outputLength)
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
				int n = RSA_public_encrypt(static_cast<int>(maxSize), _pBuf, output, const_cast<RSA*>(_pRSA), mapPaddingMode(_paddingMode));
				if (n == -1)
					throwError();
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
		poco_assert (length >= static_cast<std::streamsize>(blockSize()));
		poco_assert (static_cast<std::size_t>(_pos) <= maxDataSize());
		int rc = 0;
		if (_pos > 0)
		{
			rc = RSA_public_encrypt(static_cast<int>(_pos), _pBuf, output, const_cast<RSA*>(_pRSA), mapPaddingMode(_paddingMode));
			if (rc == -1) throwError();
		}
		return rc;
	}


	class RSADecryptImpl: public CryptoTransform
	{
	public:
		RSADecryptImpl(const RSA* pRSA, RSAPaddingMode paddingMode);
		~RSADecryptImpl();

		std::size_t blockSize() const;
		std::string getTag(std::size_t);
		void setTag(const std::string&);

		std::streamsize transform(
			const unsigned char* input,
			std::streamsize		 inputLength,
			unsigned char*		 output,
			std::streamsize		 outputLength);

		std::streamsize finalize(
			unsigned char*	output,
			std::streamsize length);

	private:
		const RSA*      _pRSA;
		RSAPaddingMode  _paddingMode;
		std::streamsize _pos;
		unsigned char*  _pBuf;
	};


	RSADecryptImpl::RSADecryptImpl(const RSA *pRSA, RSAPaddingMode paddingMode) :
			_pRSA(pRSA),
			_paddingMode(paddingMode),
			_pos(0),
			_pBuf(nullptr)
	{
		_pBuf = new unsigned char[blockSize()];
	}


	RSADecryptImpl::~RSADecryptImpl()
	{
		delete [] _pBuf;
	}


	std::size_t RSADecryptImpl::blockSize() const
	{
		return RSA_size(_pRSA);
	}


	std::string RSADecryptImpl::getTag(std::size_t)
	{
		return std::string();
	}


	void RSADecryptImpl::setTag(const std::string&)
	{
	}


	std::streamsize RSADecryptImpl::transform(
		const unsigned char* input,
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
				int tmp = RSA_private_decrypt(static_cast<int>(rsaSize), _pBuf, output, const_cast<RSA*>(_pRSA), mapPaddingMode(_paddingMode));
				if (tmp == -1)
					throwError();
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
		poco_assert (length >= static_cast<std::streamsize>(blockSize()));
		int rc = 0;
		if (_pos > 0)
		{
			rc = RSA_private_decrypt(static_cast<int>(_pos), _pBuf, output, const_cast<RSA*>(_pRSA), mapPaddingMode(_paddingMode));
			if (rc == -1)
				throwError();
		}
		return rc;
	}


#endif // POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)

}


RSACipherImpl::RSACipherImpl(const RSAKey& key, RSAPaddingMode paddingMode):
	_key(key),
	_paddingMode(paddingMode)
{
}


RSACipherImpl::~RSACipherImpl()
{
}


CryptoTransform::Ptr RSACipherImpl::createEncryptor()
{
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	return new RSAEncryptImpl(_key.impl()->getEVPPKey(), _paddingMode);
#else
	return new RSAEncryptImpl(_key.impl()->getRSA(), _paddingMode);
#endif
}


CryptoTransform::Ptr RSACipherImpl::createDecryptor()
{
#if POCO_OPENSSL_VERSION_PREREQ(3, 0, 0)
	return new RSADecryptImpl(_key.impl()->getEVPPKey(), _paddingMode);
#else
	return new RSADecryptImpl(_key.impl()->getRSA(), _paddingMode);
#endif
}


} // namespace Poco::Crypto
