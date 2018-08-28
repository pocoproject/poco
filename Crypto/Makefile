#
# Makefile
#
# Makefile for Poco Crypto
#

include $(POCO_BASE)/build/rules/global

SYSLIBS += -l$(POCO_SSL_LIB) -l$(POCO_CRYPTO_LIB)

objects = Cipher CipherFactory CipherImpl CipherKey CipherKeyImpl \
	CryptoException CryptoStream CryptoTransform ECDSADigestEngine \
	ECKey ECKeyImpl EVPPKey KeyPair KeyPairImpl PKCS12Container \
	RSACipherImpl RSAKey RSAKeyImpl RSADigestEngine DigestEngine \
	X509Certificate OpenSSLInitializer

target         = PocoCrypto
target_version = $(LIBVERSION)
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
