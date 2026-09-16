//
// DeviceBindingProxyFactory.h
//
// Package: Generated
// Module:  DeviceBindingProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef DeviceBindingProxyFactory_INCLUDED
#define DeviceBindingProxyFactory_INCLUDED


#include "ONVIF/Device/DeviceBindingProxy.h"
#include "Poco/RemotingNG/ProxyFactory.h"


namespace ONVIF {
namespace Device {


class DeviceBindingProxyFactory: public Poco::RemotingNG::ProxyFactory
{
public:
	DeviceBindingProxyFactory();
		/// Creates a DeviceBindingProxyFactory.

	~DeviceBindingProxyFactory();
		/// Destroys the DeviceBindingProxyFactory.

	Poco::RemotingNG::Proxy* createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const;
		/// Creates and returns a new Proxy object.

};


} // namespace Device
} // namespace ONVIF


#endif // DeviceBindingProxyFactory_INCLUDED

