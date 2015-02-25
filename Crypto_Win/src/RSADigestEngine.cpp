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
#include "Poco/Buffer.h"
#include "Poco/Format.h"
#include "Poco/Error.h"


namespace Poco {
namespace Crypto {


RSADigestEngine::RSADigestEngine(const RSAKey& key, DigestType digestType):
	_key(key),
	_engine(digestType == DIGEST_MD5 ?
		static_cast<Poco::DigestEngine&>(_md5Engine) :
		static_cast<Poco::DigestEngine&>(_sha1Engine)),
	_type(digestType == DIGEST_MD5 ? szOID_RSA_MD5RSA : szOID_RSA_SHA1RSA),
	_hCertStore(CertOpenSystemStore(0, "MY"))
{
	if (!_hCertStore)
	{
		error("The \"MY\" certificate store could not be opened.");
	}
}


RSADigestEngine::~RSADigestEngine()
{
	if (_hCertStore)
		CertCloseStore(_hCertStore, CERT_CLOSE_STORE_CHECK_FLAG);
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
		sign();
	}
	return _signature;
}

	
bool RSADigestEngine::verify(const DigestEngine::Digest& sig)
{
	digest();

	DWORD cbDecoded = 0;
	Poco::Buffer<BYTE> decodedBuf;
	CRYPT_VERIFY_MESSAGE_PARA verifyParams;
	verifyParams.cbSize = sizeof(CRYPT_VERIFY_MESSAGE_PARA);
	verifyParams.dwMsgAndCertEncodingType = PKCS_7_ASN_ENCODING | X509_ASN_ENCODING;
	verifyParams.hCryptProv = 0;
	verifyParams.pfnGetSignerCertificate = NULL;
	verifyParams.pvGetArg = NULL;

	DWORD rc = CryptVerifyMessageSignature(&verifyParams,
		0, &sig[0], static_cast<DWORD>(sig.size()),
		NULL, &cbDecoded, NULL);
	if (rc)
	{
		decodedBuf.resize(cbDecoded);
		rc = CryptVerifyMessageSignature(&verifyParams,
			0, &sig[0], static_cast<DWORD>(sig.size()),
			decodedBuf.begin(), &cbDecoded, NULL);
	}

	if (!rc)
	{
		error("Verification message failed. \n");
	}

	return (cbDecoded == _digest.size()) &&
		(std::memcmp(decodedBuf.begin(), &_digest[0], cbDecoded) == 0);
}


void RSADigestEngine::updateImpl(const void* data, std::size_t length)
{
	_engine.update(data, length);
}


void RSADigestEngine::sign()
{
	PCCERT_CONTEXT pSignerCert = NULL;
	pSignerCert = CertFindCertificateInStore(_hCertStore,
		PKCS_7_ASN_ENCODING | X509_ASN_ENCODING, 0,
		/*???*/CERT_FIND_ANY, /*???*/NULL, NULL);

	if (!pSignerCert)
	{
		error("Signer certificate not found.");
	}

	CRYPT_SIGN_MESSAGE_PARA  sigParams;
	sigParams.cbSize = sizeof(CRYPT_SIGN_MESSAGE_PARA);
	sigParams.dwMsgEncodingType = PKCS_7_ASN_ENCODING | X509_ASN_ENCODING;
	sigParams.pSigningCert = pSignerCert;
	sigParams.HashAlgorithm.pszObjId = _type;
	sigParams.HashAlgorithm.Parameters.cbData = NULL;
	sigParams.cMsgCert = 1;
	sigParams.rgpMsgCert = &pSignerCert;
	sigParams.cAuthAttr = 0;
	sigParams.dwInnerContentType = 0;
	sigParams.cMsgCrl = 0;
	sigParams.cUnauthAttr = 0;
	sigParams.dwFlags = 0;
	sigParams.pvHashAuxInfo = NULL;
	sigParams.rgAuthAttr = NULL;

	const BYTE* messageArray[1] = { &_digest[0] };
	DWORD messageSizeArray[1] = { static_cast<DWORD>(_digest.size()) };
	DWORD cbSignedMessageBlob = 0;
	if (CryptSignMessage(&sigParams,
		FALSE, 1, messageArray, messageSizeArray,
		NULL, &cbSignedMessageBlob))
	{
		if (cbSignedMessageBlob != _signature.size() && cbSignedMessageBlob > 0)
			_signature.resize(cbSignedMessageBlob, false);
		else if (cbSignedMessageBlob == 0)
			error("Cannot obtain size of the signed message BLOB");

		if (!CryptSignMessage(&sigParams,
			FALSE, 1, messageArray, messageSizeArray,
			&_signature[0], &cbSignedMessageBlob))
		{
			error("Cannot get signed BLOB");
		}
	}
	else
	{
		if (pSignerCert)
			CertFreeCertificateContext(pSignerCert);

		error("Getting signed BLOB size failed");
	}

	if (pSignerCert)
		CertFreeCertificateContext(pSignerCert);
}


} } // namespace Poco::Crypto
