#
# Makefile
#
# Makefile for Poco Remoting NG
#

include $(POCO_BASE)/build/rules/global

SHAREDOPT_CXX   += -DRemotingNG_EXPORTS

objects = SerializerBase Serializer Deserializer \
	BinarySerializer BinaryDeserializer \
	Transport TransportFactory TransportFactoryManager \
	ServerTransport Listener Context \
	Authorizer Authenticator Credentials \
	Proxy ProxyFactory ProxyFactoryManager \
	Identifiable RemoteObject Skeleton MethodHandler \
	EventDispatcher EventListener EventSubscriber \
	ORB \
	AttributedObject RemotingException URIUtility

target         = PocoRemotingNG
target_version = $(LIBVERSION)
target_libs    = PocoFoundation $(POCO_LICENSING)

include $(POCO_BASE)/build/rules/lib
