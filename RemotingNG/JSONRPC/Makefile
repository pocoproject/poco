#
# Makefile
#
# Makefile for Poco RemotingNG JSONRPC Transport
#

include $(POCO_BASE)/build/rules/global

objects = Deserializer Listener Serializer Transport TransportFactory CookieStore \
	JSONRPCRequestHandler JSONRPCRequestHandlerFactory \
	JSValue JSScalar JSObject JSArray JSONParser

target         = PocoRemotingNGJSONRPC
target_version = $(LIBVERSION)
target_libs    = PocoRemotingNG PocoNet PocoFoundation $(POCO_LICENSING)

include $(POCO_BASE)/build/rules/lib
