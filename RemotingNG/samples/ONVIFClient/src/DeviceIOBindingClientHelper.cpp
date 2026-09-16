//
// DeviceIOBindingClientHelper.cpp
//
// Package: Generated
// Module:  DeviceIOBindingClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "ONVIF/DeviceIO/DeviceIOBindingClientHelper.h"
#include "ONVIF/DeviceIO/DeviceIOBindingProxyFactory.h"
#include "Poco/SingletonHolder.h"


namespace ONVIF {
namespace DeviceIO {


namespace
{
	Poco::SingletonHolder<DeviceIOBindingClientHelper> shDeviceIOBindingClientHelper;
}


DeviceIOBindingClientHelper::DeviceIOBindingClientHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	_pORB->registerProxyFactory("ONVIF.DeviceIO.DeviceIOBinding", new DeviceIOBindingProxyFactory);
}


DeviceIOBindingClientHelper::~DeviceIOBindingClientHelper()
{
}


IDeviceIOBinding::Ptr DeviceIOBindingClientHelper::findImpl(const std::string& uri)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);
	return pInterface.cast<IDeviceIOBinding>();
}


IDeviceIOBinding::Ptr DeviceIOBindingClientHelper::findImpl(const std::string& uri, const std::string& protocol)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, IDeviceIOBinding::remoting__typeId(), protocol);
	return pInterface.cast<IDeviceIOBinding>();
}


DeviceIOBindingClientHelper& DeviceIOBindingClientHelper::instance()
{
	return *shDeviceIOBindingClientHelper.get();
}


} // namespace DeviceIO
} // namespace ONVIF

