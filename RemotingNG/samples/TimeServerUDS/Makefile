#
# Makefile
#
# Makefile for RemotingNG TimeServerUDS sample
#

include $(POCO_BASE)/build/rules/global

objects = TimeServer ITimeService TimeService TimeServiceEventDispatcher \
	TimeServiceRemoteObject TimeServiceServerHelper TimeServiceSkeleton

target         = TimeServer
target_version = 1
target_libs    = PocoRemotingNGTCP PocoRemotingNG PocoUtil PocoNet PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
