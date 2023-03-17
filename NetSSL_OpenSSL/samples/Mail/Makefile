#
# Makefile
#
# Makefile for Poco Mail
#

include $(POCO_BASE)/build/rules/global

# Note: linking order is important, do not change it.
ifeq ($(POCO_CONFIG),FreeBSD)
SYSLIBS += -lssl -lcrypto -lz
else
ifeq ($(POCO_CONFIG),QNX)
SYSLIBS += -lssl -lcrypto -lz
else
ifeq ($(findstring AIX, $(POCO_CONFIG)), AIX)
SYSLIBS += -lssl_a -lcrypto_a -lz -ldl
else
SYSLIBS += -lssl -lcrypto -lz -ldl
endif
endif
endif

objects = Mail

target         = Mail
target_version = 1
target_libs    = PocoNetSSL PocoNet PocoCrypto PocoUtil PocoJSON PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
