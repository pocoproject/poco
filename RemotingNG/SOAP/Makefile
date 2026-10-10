#
# Makefile
#
# Makefile for Poco RemotingNG SOAP Transport
#

include $(POCO_BASE)/build/rules/global

objects = \
	Deserializer \
	Listener \
	Serializer \
	SOAPContentHandler \
	SOAPRequestHandler \
	SOAPFaultException \
	Transport \
	TransportFactory \
	CookieStore \
	WSDLRequestHandler \
	XMLElement

target         = PocoRemotingNGSOAP
target_version = $(LIBVERSION)
target_libs    = PocoRemotingNG PocoNet PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/lib
