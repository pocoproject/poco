#
# Makefile
#
# Makefile for Poco Remoting testsuite
#

include $(POCO_BASE)/build/rules/global

objects = \
	Driver \
	RemotingTest \
	EventFilterTest \
	URIUtilityTest \
	RemotingTestSuite \
	Tester \
	ITester \
	TesterEventSubscriber \
	TesterClientHelper \
	TesterEventDispatcher \
	TesterProxy \
	TesterProxyFactory \
	TesterRemoteObject \
	TesterServerHelper \
	TesterSkeleton

target         = testrunner
target_version = 1
target_libs    = PocoRemotingNG PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
