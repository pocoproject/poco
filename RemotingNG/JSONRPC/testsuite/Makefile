#
# Makefile
#
# Makefile for Poco RemotingNG JSONRPC Transport testsuite
#

include $(POCO_BASE)/build/rules/global

objects = \
	Driver \
	RemotingTest \
	SerializerTest \
	DeserializerTest \
	JSONParserTest \
	JSONRPCTestSuite \
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
target_libs    = PocoRemotingNGJSONRPC PocoRemotingNG PocoNet PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
