#
# Makefile
#
# Makefile for Poco CodeGen
#

include $(POCO_BASE)/build/rules/global

ifdef POCO_XSD_ENABLE_HTTPS
EXTRA_CXXFLAGS += -DPOCO_XSD_ENABLE_HTTPS
HTTPSLIBS = PocoNetSSL PocoCrypto
endif

objects = AbstractMethod BuiltinTypes \
	ClassInfo Constructor CppGen \
	CppWriter Destructor main \
	MethodInfo Parameter PropertyHolder \
	SchemaInfo TypeInfo Utility Variable

target         = XSDGen
target_version = 1
target_libs    = PocoXSDParser PocoXSDTypes PocoUtil $(HTTPSLIBS) PocoNet PocoXML PocoJSON PocoFoundation

include $(POCO_BASE)/build/rules/exec
