//
// DeviceBindingProxyFactory.cpp
//
// Package: Generated
// Module:  DeviceBindingProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "ONVIF/Device/DeviceBindingProxyFactory.h"


namespace ONVIF {
namespace Device {


DeviceBindingProxyFactory::DeviceBindingProxyFactory():
	Poco::RemotingNG::ProxyFactory()

{
}


DeviceBindingProxyFactory::~DeviceBindingProxyFactory()
{
}


Poco::RemotingNG::Proxy* DeviceBindingProxyFactory::createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const
{
	return new ONVIF::Device::DeviceBindingProxy(oid);
}


} // namespace Device
} // namespace ONVIF

