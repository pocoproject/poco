module;

#ifdef ENABLE_CRYPTO
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CipherImpl.h"
#include "Poco/Crypto/CipherKey.h"
#include "Poco/Crypto/CipherKeyImpl.h"
#include "Poco/Crypto/CryptoException.h"
#include "Poco/Crypto/Crypto.h"
#include "Poco/Crypto/CryptoStream.h"
#include "Poco/Crypto/CryptoTransform.h"
#include "Poco/Crypto/DigestEngine.h"
#include "Poco/Crypto/ECDSADigestEngine.h"
#include "Poco/Crypto/ECKey.h"
#include "Poco/Crypto/ECKeyImpl.h"
#include "Poco/Crypto/Envelope.h"
#include "Poco/Crypto/EVPCipherImpl.h"
#include "Poco/Crypto/EVPPKey.h"
#include "Poco/Crypto/KeyPair.h"
#include "Poco/Crypto/KeyPairImpl.h"
#include "Poco/Crypto/OpenSSLInitializer.h"
#include "Poco/Crypto/PKCS12Container.h"
#include "Poco/Crypto/RSACipherImpl.h"
#include "Poco/Crypto/RSADigestEngine.h"
#include "Poco/Crypto/RSAKey.h"
#include "Poco/Crypto/RSAKeyImpl.h"
#include "Poco/Crypto/X509Certificate.h"
#endif

export module Poco.Crypto;

export namespace Poco::Crypto {
	#ifdef ENABLE_CRYPTO
	using Poco::Crypto::Cipher;
	using Poco::Crypto::CipherFactory;
	using Poco::Crypto::CipherKey;
	using Poco::Crypto::CipherKeyImpl;
	using Poco::Crypto::CryptoException;
	using Poco::Crypto::CryptoIOS;
	using Poco::Crypto::CipherImpl;
	using Poco::Crypto::CryptoInputStream;
	using Poco::Crypto::CryptoOutputStream;
	using Poco::Crypto::CryptoStreamBuf;
	using Poco::Crypto::CryptoTransform;
	using Poco::Crypto::DecryptingInputStream;
	using Poco::Crypto::DecryptingOutputStream;
	using Poco::Crypto::DigestEngine;
	using Poco::Crypto::ECDSADigestEngine;
	using Poco::Crypto::ECDSASignature;
	using Poco::Crypto::ECKey;
	using Poco::Crypto::ECKeyImpl;
	using Poco::Crypto::EVPCipherImpl;
	using Poco::Crypto::EVPPKey;
	using Poco::Crypto::EncryptingInputStream;
	using Poco::Crypto::EncryptingOutputStream;
	using Poco::Crypto::Envelope;
	using Poco::Crypto::KeyPair;
	using Poco::Crypto::KeyPairImpl;
	using Poco::Crypto::OpenSSLException;
	using Poco::Crypto::OpenSSLInitializer;
	using Poco::Crypto::PKCS12Container;
	using Poco::Crypto::RSACipherImpl;
	using Poco::Crypto::RSADigestEngine;
	using Poco::Crypto::RSAKey;
	using Poco::Crypto::RSAKeyImpl;
	using Poco::Crypto::X509Certificate;

	using Poco::Crypto::getError;
	using Poco::Crypto::initializeCrypto;
	using Poco::Crypto::uninitializeCrypto;
	#endif
}
