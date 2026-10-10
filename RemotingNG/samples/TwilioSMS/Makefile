#
# Makefile
#
# Makefile for Twilio SMS Client sample
#

include $(POCO_BASE)/build/rules/global

objects = \
	ITwilioSMSEndpoint \
	TwilioSMSEndpointClientHelper \
	TwilioSMSEndpointProxy \
	TwilioSMSEndpointProxyFactory \
	TwilioSMSClient

target         = TwilioSMSClient
target_version = 1
target_libs    = PocoRemotingNGREST PocoRemotingNG PocoCrypto PocoNetSSL PocoNet PocoFoundation

include $(POCO_BASE)/build/rules/exec
