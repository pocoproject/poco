#
# Makefile
#
# Makefile for Poco RemotingNG SOAP Transport testsuite
#

include $(POCO_BASE)/build/rules/global

objects = \
	Driver \
	RemotingTest \
	SerializerTest \
	DeserializerTest \
	SOAPTestSuite \
	Tester \
	ITester \
	TesterClientHelper \
	TesterProxy \
	TesterProxyFactory \
	TesterRemoteObject \
	TesterServerHelper \
	TesterSkeleton

target         = testrunner
target_version = 1
target_libs    = PocoRemotingNGSOAP PocoRemotingNG PocoNet PocoXML PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
