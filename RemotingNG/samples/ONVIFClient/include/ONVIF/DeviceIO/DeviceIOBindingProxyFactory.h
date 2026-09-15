//
// DeviceIOBindingProxyFactory.h
//
// Package: Generated
// Module:  DeviceIOBindingProxyFactory
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef DeviceIOBindingProxyFactory_INCLUDED
#define DeviceIOBindingProxyFactory_INCLUDED


#include "ONVIF/DeviceIO/DeviceIOBindingProxy.h"
#include "Poco/RemotingNG/ProxyFactory.h"


namespace ONVIF {
namespace DeviceIO {


class DeviceIOBindingProxyFactory: public Poco::RemotingNG::ProxyFactory
{
public:
	DeviceIOBindingProxyFactory();
		/// Creates a DeviceIOBindingProxyFactory.

	~DeviceIOBindingProxyFactory();
		/// Destroys the DeviceIOBindingProxyFactory.

	Poco::RemotingNG::Proxy* createProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid) const;
		/// Creates and returns a new Proxy object.

};


} // namespace DeviceIO
} // namespace ONVIF


#endif // DeviceIOBindingProxyFactory_INCLUDED

