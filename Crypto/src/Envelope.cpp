//
// Envelope.cpp
//
// Library: Crypto
// Package: Envelope
// Module:  Envelope
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/Envelope.h"

namespace Poco {
namespace Crypto {


Envelope::Envelope(int cipherNID): _pCipher(EVP_get_cipherbynid(cipherNID)),
	_pCtx(EVP_CIPHER_CTX_new())
{
	poco_check_ptr(_pCipher);
	poco_check_ptr(_pCtx);
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
	if (1 != EVP_CIPHER_CTX_reset(_pCtx))
		handleErrors(std::string("Envelope():EVP_CIPHER_CTX_reset()"));
#else
	EVP_CIPHER_CTX_init(_pCtx);
#endif
	_iv.resize(ivSize(), 0);
}


Envelope::Envelope(const EVPPKey& key, int cipherNID):
	Envelope(cipherNID)
{
	addKey(key);
}


Envelope::Envelope(const EVPPKeyVec& keys, int cipherNID):
	Envelope(cipherNID)
{
	for (const auto& k : keys) addKey(k);
}


Envelope::~Envelope()
{
	for (auto& pK : _pubKeys)
		EVP_PKEY_free(pK);
	EVP_CIPHER_CTX_free(_pCtx);
}


void Envelope::addKey(const EVPPKey& key)
{
	EVP_PKEY* pKey;
	_pubKeys.push_back(EVPPKey::duplicate((const EVP_PKEY*)key, &pKey));
	_encKeys.emplace_back(EVP_PKEY_size(_pubKeys.back()));
}


const Envelope::ByteVec& Envelope::seal(const ByteVec& plainData)
{
	std::vector<Byte*> pEncKeys(_encKeys.size(), 0);
	std::vector<int> encKeysSizes(_encKeys.size(), 0);
	int i = 0;
	for (const auto& k : _encKeys)
		pEncKeys[i++] = new Byte[k.size()];

	int noOfKeys = static_cast<int>(_pubKeys.size());
	if (_encKeys.size() != EVP_SealInit(_pCtx, _pCipher, &pEncKeys[0], &encKeysSizes[0], &_iv[0], &_pubKeys[0], noOfKeys))
	{
		i = 0;
		for (; i < _encKeys.size(); ++i) delete [] pEncKeys[i];
		handleErrors(std::string("Envelope::seal():EVP_SealInit()"));
	}
	i = 0;
	for (auto& k : _encKeys)
	{
		if (encKeysSizes[i] != k.size())
			k.resize(encKeysSizes[i]);
		std::memcpy(&k[0], pEncKeys[i], encKeysSizes[i]);
		++i;
	}

	i = 0;
	for (; i < _encKeys.size(); ++i) delete [] pEncKeys[i];

	int cipherTextLen = 0, len = 0;
	int plainDataSize = static_cast<int>(plainData.size());
    _encContent.resize(plainDataSize + blockSize());
	if (1 != EVP_SealUpdate(_pCtx, &_encContent[0], &len, &plainData[0], plainDataSize))
		handleErrors(std::string("Envelope::seal():EVP_SealUpdate()"));
	
	cipherTextLen = len;
	poco_assert (cipherTextLen < _encContent.size());

	if(1 != EVP_SealFinal(_pCtx, &_encContent[len], &len))
		handleErrors(std::string("Envelope::seal():EVP_SealFinal()"));
	cipherTextLen += len;
	poco_assert (cipherTextLen <= _encContent.size());
	_encContent.resize(cipherTextLen);

	return _encContent;
}


const Envelope::ByteVec& Envelope::seal(const std::string& plainText)
{
	return seal(ByteVec(plainText.begin(), plainText.end()));
}


Envelope::ByteVec Envelope::open(const EVPPKey& privKey, const ByteVec& encKey, const ByteVec& iv)
{
	if (iv.size() > 0) _iv = iv;
	int encContentLen = static_cast<int>(_encContent.size());
	int blockSz = blockSize();
	int mod = encContentLen % blockSz;
	if (mod || (encContentLen < blockSz))
	{
		throw Poco::InvalidArgumentException(
			Poco::format("Envelope::open(): bad encrypted buffer size: %z (must be N x %d)",
				_encContent.size(), blockSz));
	}

	int encKeyLen = static_cast<int>(encKey.size());
	EVP_PKEY* pKey = const_cast<EVP_PKEY*>((const EVP_PKEY*)privKey);
	if (1 != EVP_OpenInit(_pCtx, _pCipher, &encKey[0], encKeyLen, &_iv[0], pKey))
		handleErrors(std::string("Envelope::open():EVP_OpenInit()"));

	ByteVec plainData(_encContent.size()+blockSz, 0);
	int len = 0;
	if(1 != EVP_OpenUpdate(_pCtx, &plainData[0], &len, &_encContent[0], encContentLen))
		handleErrors(std::string("Envelope::open():EVP_OpenUpdate()"));
	int totalLen = len;
	
	if(1 != EVP_OpenFinal(_pCtx, &plainData[len], &len))
		handleErrors(std::string("Envelope::open():EVP_OpenFinal()"));
	totalLen += len;
	plainData.resize(totalLen);
	return plainData;
}


void Envelope::handleErrors(std::string&& msg)
{
	unsigned long err;
	while ((err = ERR_get_error()))
	{
		if (!msg.empty()) msg.append("\n");
		msg.append(ERR_error_string(err, 0));
	}
	throw CryptoException(msg);
}


} } // namespace Poco::Crypto
