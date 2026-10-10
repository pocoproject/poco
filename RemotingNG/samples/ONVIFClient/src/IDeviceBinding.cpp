//
// IDeviceBinding.cpp
//
// Package: Generated
// Module:  IDeviceBinding
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "ONVIF/Device/IDeviceBinding.h"


namespace ONVIF {
namespace Device {


IDeviceBinding::IDeviceBinding():
	Poco::RefCountedObject()

{
}


IDeviceBinding::~IDeviceBinding()
{
}


const Poco::RemotingNG::Identifiable::TypeId& IDeviceBinding::remoting__typeId()
{
	static const std::string REMOTING__TYPE_ID("ONVIF.Device.DeviceBinding");
	return REMOTING__TYPE_ID;
}


} // namespace Device
} // namespace ONVIF

