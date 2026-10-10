#
# Makefile
#
# Makefile for Poco RemotingNG REST Transport
#

include $(POCO_BASE)/build/rules/global

objects = Listener Transport TransportFactory CookieStore \
	RESTRequestHandler RESTRequestHandlerFactory \
	JSValue JSScalar JSObject JSArray JSONParser \
	Serializer Deserializer \
	JSONSerializer JSONDeserializer \
	ScalarSerializer ScalarDeserializer \
	PathSerializer PathDeserializer \
	FormSerializer FormDeserializer \
	HeaderSerializer HeaderDeserializer \
	RawSerializer RawDeserializer \
	RESTServerException

target         = PocoRemotingNGREST
target_version = $(LIBVERSION)
target_libs    = PocoRemotingNG PocoNet PocoFoundation

include $(POCO_BASE)/build/rules/lib
