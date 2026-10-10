//
// DeviceBindingClientHelper.cpp
//
// Package: Generated
// Module:  DeviceBindingClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "ONVIF/Device/DeviceBindingClientHelper.h"
#include "ONVIF/Device/DeviceBindingProxyFactory.h"
#include "Poco/SingletonHolder.h"


namespace ONVIF {
namespace Device {


namespace
{
	Poco::SingletonHolder<DeviceBindingClientHelper> shDeviceBindingClientHelper;
}


DeviceBindingClientHelper::DeviceBindingClientHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	_pORB->registerProxyFactory("ONVIF.Device.DeviceBinding", new DeviceBindingProxyFactory);
}


DeviceBindingClientHelper::~DeviceBindingClientHelper()
{
}


IDeviceBinding::Ptr DeviceBindingClientHelper::findImpl(const std::string& uri)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);
	return pInterface.cast<IDeviceBinding>();
}


IDeviceBinding::Ptr DeviceBindingClientHelper::findImpl(const std::string& uri, const std::string& protocol)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, IDeviceBinding::remoting__typeId(), protocol);
	return pInterface.cast<IDeviceBinding>();
}


DeviceBindingClientHelper& DeviceBindingClientHelper::instance()
{
	return *shDeviceBindingClientHelper.get();
}


} // namespace Device
} // namespace ONVIF

