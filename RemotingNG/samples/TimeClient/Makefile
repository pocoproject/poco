#
# Makefile
#
# Makefile for RemotingNG TimeClient sample
#

include $(POCO_BASE)/build/rules/global

INCLUDE += -I../TimeServer/include

objects = ITimeService TimeClient TimeServiceClientHelper TimeServiceProxy TimeServiceProxyFactory

target         = TimeClient
target_version = 1
target_libs    = PocoRemotingNGSOAP PocoRemotingNG PocoUtil PocoNet PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
