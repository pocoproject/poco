#
# Makefile
#
# Makefile for RemotingNG TimeClient sample
#

include $(POCO_BASE)/build/rules/global

objects = ITimeService TimeClient TimeServiceClientHelper TimeServiceEventSubscriber \
	TimeServiceProxy TimeServiceProxyFactory

target         = TimeClient
target_version = 1
target_includes = ../TimeServerTCP/include
target_libs    = PocoRemotingNGTCP PocoRemotingNG PocoUtil PocoNet PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
