#
# Makefile
#
# Makefile for CheckVat sample
#

include $(POCO_BASE)/build/rules/global

ifdef CHECKVAT_ENABLE_HTTPS
CXXFLAGS += -DCHECKVAT_ENABLE_HTTPS
HTTPSLIBS = PocoNetSSL PocoCrypto
endif

objects = \
	CheckVat \
	CheckVatApprox \
	CheckVatApproxResponse \
	CheckVatResponse \
	CheckVatService \
	CheckVatServiceClientHelper \
	CheckVatServiceProxy \
	CheckVatServiceProxyFactory \
	ICheckVatService \
	CheckVatClient

target         = CheckVat
target_version = 1
target_libs    = PocoRemotingNGSOAP PocoRemotingNG $(HTTPSLIBS) PocoUtil PocoNet PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
