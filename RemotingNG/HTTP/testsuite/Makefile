#
# Makefile
#
# Makefile for Poco RemotingNG HTTP Transport testsuite
#

include $(POCO_BASE)/build/rules/global

objects = \
	Driver \
	RemotingTest \
	HTTPTestSuite \
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
target_libs    = PocoRemotingNGHTTP PocoRemotingNG PocoNet PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
