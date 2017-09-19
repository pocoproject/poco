#
# Makefile
#
# Makefile for ApacheConnector
#

include $(POCO_BASE)/build/rules/global

SYSFLAGS += -I/usr/include/apache2 \
	-I/usr/include/apr-1.0 -I/usr/include/apr-1 -I/usr/include/apr-0

ifeq ($(OSNAME),Darwin)
SHAREDLIBLINKEXT = .so
DYLIB += -flat_namespace -undefined suppress
endif

objects = ApacheRequestHandlerFactory  \
	ApacheServerRequest ApacheServerResponse \
	ApacheStream ApacheConnector ApacheChannel \
	ApacheApplication ApacheChannel

target         = mod_poco
target_version = 1
target_libs    = PocoUtil PocoNet PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/dylib
