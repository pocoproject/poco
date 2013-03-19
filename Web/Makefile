#
# Makefile
#
# $Id: //poco/Main/template/library.make#3 $
#
# Makefile for Poco Web
#

include $(POCO_BASE)/build/rules/global

objects = SOAPBody SOAPElement SOAPEnvelope SOAPHeader SOAPMessage \
	JSONCondenser JSONConfiguration JSONEntity JSONHandler \
	JSONParser JSONPrettyPrinter JSONPrinter \
	DirectAction DirectHandler DirectResponse

target         = PocoWeb
target_version = $(LIBVERSION)
target_libs    = PocoFoundation PocoXML PocoNet

include $(POCO_BASE)/build/rules/lib
