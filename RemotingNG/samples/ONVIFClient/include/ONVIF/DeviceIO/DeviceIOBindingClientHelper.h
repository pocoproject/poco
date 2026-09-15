//
// DeviceIOBindingClientHelper.h
//
// Package: Generated
// Module:  DeviceIOBindingClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef DeviceIOBindingClientHelper_INCLUDED
#define DeviceIOBindingClientHelper_INCLUDED


#include "ONVIF/DeviceIO/IDeviceIOBinding.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "Poco/RemotingNG/ORB.h"


namespace ONVIF {
namespace DeviceIO {


class DeviceIOBindingClientHelper
{
public:
	DeviceIOBindingClientHelper();
		/// Creates a DeviceIOBindingClientHelper.

	~DeviceIOBindingClientHelper();
		/// Destroys the DeviceIOBindingClientHelper.

	static IDeviceIOBinding::Ptr find(const std::string& uri);
		/// Return an interface for the service object identified by the given URI.
		///
		/// Depending on whether the service object has been registered on the same ORB, or not,
		/// the ORB will either return a RemoteObject (with forwards calls locally, without the
		/// need for serialization/deserialization), or a Proxy.
		///
		/// The URI must have the following format: <scheme>://<authority>/<protocol>/<typeId>/<objectId>

	static IDeviceIOBinding::Ptr find(const std::string& uri, const std::string& protocol);
		/// Return a Proxy for the service object identified by the given URI.
		///
		/// The given protocol name is used to determine the Transport used by
		/// the Proxy. This is used for objects identified by URIs that do not
		/// follow the standard Remoting URI structure.

private:
	IDeviceIOBinding::Ptr findImpl(const std::string& uri);

	IDeviceIOBinding::Ptr findImpl(const std::string& uri, const std::string& protocol);

	static DeviceIOBindingClientHelper& instance();

	Poco::RemotingNG::ORB* _pORB;
};


inline IDeviceIOBinding::Ptr DeviceIOBindingClientHelper::find(const std::string& uri)
{
	return DeviceIOBindingClientHelper::instance().findImpl(uri);
}


inline IDeviceIOBinding::Ptr DeviceIOBindingClientHelper::find(const std::string& uri, const std::string& protocol)
{
	return DeviceIOBindingClientHelper::instance().findImpl(uri, protocol);
}


} // namespace DeviceIO
} // namespace ONVIF


#endif // DeviceIOBindingClientHelper_INCLUDED

