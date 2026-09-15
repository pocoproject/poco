#
# Makefile
#
# Makefile for Poco RemotingNG REST Transport testsuite
#

include $(POCO_BASE)/build/rules/global

objects = \
	Driver \
	JSONParserTest \
	JSONSerializerTest \
	JSONDeserializerTest \
	PathSerializerTest \
	PathDeserializerTest \
	HeaderSerializerTest \
	HeaderDeserializerTest \
	FormSerializerTest \
	FormDeserializerTest \
	RawSerializerTest \
	RawDeserializerTest \
	SerializerTest \
	DeserializerTest \
	RESTTestSuite \
	Tester \
	ITester \
	TesterProxy \
	TesterProxyFactory \
	TesterClientHelper \
	TesterSkeleton \
	TesterRemoteObject \
	TesterServerHelper \
	IAuthTester \
	AuthTesterClientHelper \
	AuthTesterProxy \
	AuthTesterProxyFactory \
	AuthTesterRemoteObject \
	AuthTesterServerHelper \
	AuthTesterSkeleton \
	RemotingTest

target         = testrunner
target_version = 1
target_libs    = PocoRemotingNGREST PocoRemotingNG PocoNet PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
