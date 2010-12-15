#
# Makefile
#
# $Id: //poco/1.4/Crypto/Makefile#1 $
#
# Makefile for Poco Crypto
#

include $(POCO_BASE)/build/rules/global

SYSLIBS += -lssl -lcrypto

objects = Cipher CipherFactory CipherImpl CipherKey CipherKeyImpl CryptoStream CryptoTransform \
	RSACipherImpl RSAKey RSAKeyImpl RSADigestEngine \
	X509Certificate OpenSSLInitializer

target         = PocoCrypto
target_version = $(LIBVERSION)
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
