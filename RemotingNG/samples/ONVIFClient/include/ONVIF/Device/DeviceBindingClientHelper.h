//
// DeviceBindingClientHelper.h
//
// Package: Generated
// Module:  DeviceBindingClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef DeviceBindingClientHelper_INCLUDED
#define DeviceBindingClientHelper_INCLUDED


#include "ONVIF/Device/IDeviceBinding.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"


namespace ONVIF {
namespace Device {


class DeviceBindingClientHelper
{
public:
	DeviceBindingClientHelper();
		/// Creates a DeviceBindingClientHelper.

	~DeviceBindingClientHelper();
		/// Destroys the DeviceBindingClientHelper.

	static IDeviceBinding::Ptr find(const std::string& uri);
		/// Return an interface for the service object identified by the given URI.
		///
		/// Depending on whether the service object has been registered on the same ORB, or not,
		/// the ORB will either return a RemoteObject (with forwards calls locally, without the
		/// need for serialization/deserialization), or a Proxy.
		///
		/// The URI must have the following format: <scheme>://<authority>/<protocol>/<typeId>/<objectId>

	static IDeviceBinding::Ptr find(const std::string& uri, const std::string& protocol);
		/// Return a Proxy for the service object identified by the given URI.
		///
		/// The given protocol name is used to determine the Transport used by
		/// the Proxy. This is used for objects identified by URIs that do not
		/// follow the standard Remoting URI structure.

private:
	IDeviceBinding::Ptr findImpl(const std::string& uri);

	IDeviceBinding::Ptr findImpl(const std::string& uri, const std::string& protocol);

	static DeviceBindingClientHelper& instance();

	Poco::RemotingNG::ORB* _pORB;
};


inline IDeviceBinding::Ptr DeviceBindingClientHelper::find(const std::string& uri)
{
	return DeviceBindingClientHelper::instance().findImpl(uri);
}


inline IDeviceBinding::Ptr DeviceBindingClientHelper::find(const std::string& uri, const std::string& protocol)
{
	return DeviceBindingClientHelper::instance().findImpl(uri, protocol);
}


} // namespace Device
} // namespace ONVIF


#endif // DeviceBindingClientHelper_INCLUDED

