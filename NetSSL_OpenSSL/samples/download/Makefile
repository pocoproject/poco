#
# Makefile
#
# Makefile for Poco download
#

include $(POCO_BASE)/build/rules/global

# Note: linking order is important, do not change it.
ifeq ($(POCO_CONFIG),FreeBSD)
SYSLIBS += -l$(POCO_SSL_LIB) -l$(POCO_CRYPTO_LIB) -lz
else
SYSLIBS += -l$(POCO_SSL_LIB) -l$(POCO_CRYPTO_LIB) -lz -ldl
endif

objects = download

target         = download
target_version = 1
target_libs    = PocoNetSSL PocoCrypto PocoNet PocoUtil PocoJSON PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
