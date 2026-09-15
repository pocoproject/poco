//
// DeviceIOBindingProxyFactory.cpp
//
// Package: Generated
// Module:  DeviceIOBindingProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "ONVIF/DeviceIO/DeviceIOBindingProxyFactory.h"


namespace ONVIF {
namespace DeviceIO {


DeviceIOBindingProxyFactory::DeviceIOBindingProxyFactory():
	Poco::RemotingNG::ProxyFactory()

{
}


DeviceIOBindingProxyFactory::~DeviceIOBindingProxyFactory()
{
}


Poco::RemotingNG::Proxy* DeviceIOBindingProxyFactory::createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const
{
	return new ONVIF::DeviceIO::DeviceIOBindingProxy(oid);
}


} // namespace DeviceIO
} // namespace ONVIF

