#
# Makefile
#
# $Id$
#
# Makefile for Poco Crypto
#

include $(POCO_BASE)/build/rules/global

SYSLIBS += -lssl -lcrypto

objects = Cipher CipherFactory CipherImpl CipherKey CipherKeyImpl CryptoStream CryptoTransform \
	RSACipherImpl RSAKey RSAKeyImpl RSADigestEngine

target         = PocoCrypto
target_version = $(LIBVERSION)
target_libs    = PocoNetSSL PocoNet PocoFoundation

include $(POCO_BASE)/build/rules/lib
