#
# Makefile
#
# Makefile for Poco Crypto
#

include $(POCO_BASE)/build/rules/global

GLOBAL_SYSLIBS := $(SYSLIBS)
ifeq ($(findstring AIX, $(POCO_CONFIG)), AIX)
SYSLIBS = -lssl_a -lcrypto_a
else
SYSLIBS = -lssl -lcrypto
endif
SYSLIBS += $(GLOBAL_SYSLIBS)

objects = Cipher CipherFactory CipherImpl CipherKey CipherKeyImpl \
	CryptoException CryptoStream CryptoTransform \
	ECDSADigestEngine ECKey ECKeyImpl Envelope \
	EVPCipherImpl EVPPKey KeyPair KeyPairImpl PKCS12Container \
	RSACipherImpl RSAKey RSAKeyImpl RSADigestEngine DigestEngine \
	X509Certificate OpenSSLInitializer

target         = PocoCrypto
target_version = $(LIBVERSION)
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
