#
# Makefile
#
# Makefile for RemoteGenNG
#

include $(POCO_BASE)/build/rules/global

objects = AbstractGenerator ProxyFactoryGenerator SerializerGenerator XSDGenerator \
	ClientHelperGenerator ProxyGenerator ServerHelperGenerator \
	BundleActivatorGenerator ExtensionsGenerator \
	DeserializerGenerator RemoteGen SkeletonGenerator \
	InterfaceGenerator RemoteObjectGenerator RemoteBridgeGenerator WSDLGenerator GenUtility \
	EventDispatcherGenerator EventSubscriberGenerator \
	SwaggerGenerator

target         = RemoteGenNG
target_version = 1
target_libs    = PocoRemotingNG PocoCodeGeneration PocoNet PocoUtil PocoCppParser PocoXML PocoJSON PocoFoundation $(POCO_LICENSING)

include $(POCO_BASE)/build/rules/exec
