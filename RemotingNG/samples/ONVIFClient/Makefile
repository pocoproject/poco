#
# Makefile
#
# Makefile for RemotingNG ONVIFClient sample
#

include $(POCO_BASE)/build/rules/global

CXXFLAGS += -I../ONVIF/include

objects = \
	DeviceBindingClientHelper \
	DeviceBindingProxy \
	DeviceBindingProxyFactory \
	DeviceIOBindingClientHelper \
	DeviceIOBindingProxy \
	DeviceIOBindingProxyFactory \
	IDeviceBinding \
	IDeviceIOBinding \
	IMediaBinding \
	MediaBindingClientHelper \
	MediaBindingProxy \
	MediaBindingProxyFactory \
	ONVIFClientApp

target         = onvif
target_version = 1
target_libs    = ONVIF PocoRemotingNGSOAP PocoRemotingNG PocoUtil PocoNet PocoXML PocoFoundation

include $(POCO_BASE)/build/rules/exec
