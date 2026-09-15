#
# Makefile
#
# Makefile for Poco RemotingNG HTTP Transport
#

include $(POCO_BASE)/build/rules/global

objects = Listener Transport TransportFactory \
	HTTPRequestHandler HTTPRequestHandlerFactory 

target         = PocoRemotingNGHTTP
target_version = $(LIBVERSION)
target_libs    = PocoRemotingNG PocoNet PocoFoundation $(POCO_LICENSING)

include $(POCO_BASE)/build/rules/lib
